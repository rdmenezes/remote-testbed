#ifndef _DEVICEMANAGER_H_
#define _DEVICEMANAGER_H_

#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>

#include "macros.h"

#include "Mote.h"

namespace remote { namespace diku_mch {

class Mote;

typedef std::map<std::string, Mote *> motemap_t;

class DeviceManager
{
	public:
		static void refresh(std::string devicePath);
		static motemap_t motes;	// the current valid motes
		static motemap_t lostMotes; // motes that were just detached
		static motemap_t newMotes; // motes that were just attached

	private:
		static void readMoteDevices(std::string devicePath);
		static void updateMote(std::string& mac, std::string& path, std::string& tty);
		static std::string readMoteDeviceFile(std::string path);
};

}}

#endif
