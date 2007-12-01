#ifndef _DEVICEMANAGER_H_
#define _DEVICEMANAGER_H_

#include <map>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>

#include "Mote.h"

namespace remote { namespace diku_mch {

typedef std::map<std::string, Mote *> motemap_t;

class DeviceManager
{
	public:
		void refresh(std::string devicePath);
		motemap_t motes;	// the current valid motes
		motemap_t lostMotes; // motes that were just detached
		motemap_t newMotes; // motes that were just attached

	private:
		void readMoteDevices(std::string devicePath);
		void updateMote(std::string mac, std::string directory);
};

}}

#endif
