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

/** Refresh the mote device database.
 * Update the registered devices to match the existing system using a
 * "mark and sweep" approach. */
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
			if  (!moteI->second->isValid()) {
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

std::string DeviceManager::readMoteDeviceFile(std::string path)
{
	char buffer[1024];
	ssize_t size, i;
	int fd;

	fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd < 0)
		return std::string("");

	do {
		size = read(fd, buffer, sizeof(buffer) - 1);
	} while ((size < 0) && (errno == EAGAIN || errno == EINTR));

	/* Non-blocking, so we should have the full contents now. */
	close(fd);

	/* Sanitize the string to only hold sane ASCII characters. */
	for (i = size - 1; i >= 0; i--) {
		int c = buffer[i];

		if (c <= ' ' || c > 126) {
			/* Discard cruft at the end. */
			if (i + 1 == size) {
				size--;
				continue;
			}

			c = '_';
		} else if (c == '\'' || c == '"' || c == '\\') {
			c = '_';
		}

		buffer[i] = c;
	}
	buffer[size] = 0;

	return std::string(buffer);
}

/** Open /dev/remote and register information about each mote. */
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
		std::string moteMac = dentry->d_name;
		std::string entryPath = devicePath + "/" + dentry->d_name + "/";
		std::string moteTty = entryPath + "tty";
		std::string motePath = readMoteDeviceFile(entryPath + "path");

		if (motePath == "")
			continue;

		updateMote(moteMac, motePath, moteTty);
	}

	closedir(deviceDir);
}

/** Update the mote device.
 * Search for an existing mote based on the MAC given in the serial
 * number. */
void DeviceManager::updateMote(std::string& mac, std::string& path, std::string& tty)
{
	motemap_t::iterator m = motes.find(mac);
	if (m != motes.end()) {
		// we found a mote with the same MAC address
		// just make sure the info is up-to-date
		m->second->validate(path, tty);

	} else {
		Mote *mote = new Mote(mac, path, tty);
		if (mote->openTty() == SUCCESS) {
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
