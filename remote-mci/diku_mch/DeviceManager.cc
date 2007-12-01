#include "DeviceManager.h"

namespace remote { namespace diku_mch {

struct delMote : public std::unary_function<std::pair<const std::string, Mote *>,void>
{
	void operator()(std::pair<const std::string, Mote *> x) { delete x.second; }
};

struct invalidate : public std::unary_function<std::pair<const std::string, Mote *>,void>
{
	void operator()(std::pair<const std::string, Mote *> x)
	{ if (x.second) x.second->invalidate(); }
};

void DeviceManager::refresh(std::string devicePath)
{
	/* Invalidate all existing motes. */
	std::for_each(motes.begin(), motes.end(), invalidate());

	/* Clear new motes and delete and clear lost motes. */
	newMotes.clear();
	std::for_each(lostMotes.begin(), lostMotes.end(), delMote());
	lostMotes.clear();

	/* Read all mote devices */
	readMoteDevices(devicePath);

	/* Finally, clean up the motes that were lost since the last
	 * refresh based on whether they have been revalidated. */
	motemap_t::iterator moteI = motes.begin();
	while (moteI != motes.end()) {
		if (moteI->second) {
			if (!moteI->second->isValid()) {
				moteI->second->closeTty();
				lostMotes.insert(*moteI);
				motes.erase(moteI);
			}
		} else {
			motes.erase(moteI);
		}
		moteI++;
	}
}

void DeviceManager::readMoteDevices(std::string devicePath)
{
	struct dirent *dentry;
	DIR *deviceDir;

	deviceDir = opendir(devicePath.c_str());
	if (!deviceDir) {
		/* FIXME: Print error message if the error is not ENOENT. */
		return;
	}

	while ((dentry = readdir(deviceDir))) {
		std::string mac = dentry->d_name;

		if (mac == "." || mac == "..")
			continue;

		updateMote(mac, devicePath + "/" + mac + "/");
	}

	closedir(deviceDir);
}

/** Update the mote device.
 * Search for an existing mote based on the MAC given in the serial
 * number. */
void DeviceManager::updateMote(std::string mac, std::string directory)
{
	motemap_t::iterator m = motes.find(mac);
	if (m != motes.end()) {
		// we found a mote with the same MAC address
		// just make sure the info is up-to-date
		m->second->validate();

	} else {
		Mote *mote = new Mote(mac, directory);

		if (mote->isValid()) {
			// this must be a new mote, add it to the collection
			motemap_t::value_type elm(mac, mote);
			motes.insert(elm);
			newMotes.insert(elm);

		} else {
			delete mote;
		}
	}
}

}}
