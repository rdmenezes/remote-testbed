#include "DeviceManager.h"

namespace remote { namespace diku_mch {

struct delMote : public std::unary_function<std::pair<const uint64_t,Mote*>,void>
{
	void operator()(std::pair<const uint64_t,Mote*> x) { delete x.second; }
};

struct invalidate : public std::unary_function<std::pair<const uint64_t,Mote*>,void>
{
	void operator()(std::pair<const uint64_t,Mote*> x)
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
	while (moteI != DeviceManager::motes.end()) {
		if (moteI->second) {
			if  (!moteI->second->isValid()) {
				moteI->second->_close();
				lostMotes.insert(*moteI);
				motes.erase(moteI);
			}
		} else {
			motes.erase(moteI);
		}
		moteI++;
	}
}

/* FIXME: move (and reuse) */
/** Read from file descriptor and retry if interrupted. */
static inline ssize_t xread(int fd, void *buf, size_t len)
{
	ssize_t nr;

	while (1) {
		nr = read(fd, buf, len);
		if ((nr < 0) && (errno == EAGAIN || errno == EINTR))
			continue;
		return nr;
	}
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
		uint64_t moteMac = strtoll(dentry->d_name, NULL, 16);
		std::string entryPath = devicePath + "/" + dentry->d_name + "/";
		std::string moteTty = entryPath + "tty";
		std::string motePath;
		char buffer[1024];
		ssize_t size;
		int fd;

		entryPath += "path";
		fd = open(entryPath.c_str(), O_RDONLY | O_NONBLOCK);
		if (fd < 0)
			continue;

		size = xread(fd, buffer, sizeof(buffer) - 1);
		close(fd);

		// non-blocking, so we should have the full contents now
		while (size > 0) {
			buffer[size] = 0;
			if (!isspace(buffer[size - 1]))
				break;
			size--;
		}

		if (size <= 0)
			continue;

		motePath = buffer;

		updateMote(moteMac, motePath, moteTty);
	}

	closedir(deviceDir);
}

/** Update the mote device.
 * Search for an existing mote based on the MAC given in the serial
 * number. */
void DeviceManager::updateMote(uint64_t mac, std::string& path, std::string& tty)
{
	motemap_t::iterator m = motes.find(mac);
	if (m != motes.end()) {
		// we found a mote with the same MAC address
		// just make sure the info is up-to-date
		m->second->validate(path, tty);

	} else {
		Mote *mote = new Mote(mac, path, tty);
		if (mote->_open() == SUCCESS) {
			// this must be a new mote, add it to the collection
			motemap_t::value_type elm(mac, mote);
			motes.insert(elm);
			newMotes.insert(elm);

		} else {
			delete mote;
		}
	}
}

motemap_t DeviceManager::motes;
motemap_t DeviceManager::newMotes;
motemap_t DeviceManager::lostMotes;

}}