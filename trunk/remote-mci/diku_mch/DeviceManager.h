#ifndef _DEVICEMANAGER_H_
#define _DEVICEMANAGER_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <boost/regex.hpp>

#include "macros.h"

#include "Mote.h"
#include "Configuration.h"

namespace remote { namespace diku_mch {

typedef struct usbdevice_s
{
	uint16_t devNo;
	uint8_t	bus;
	uint8_t parent;
	uint8_t port;
	std::string serial;
} usbdevice_t;


class Mote;

typedef std::map<uint64_t,Mote*> motemap_t;
typedef std::map<std::string,usbdevice_t*> usbserialmap_t;
typedef std::map<uint64_t,usbdevice_t*> usbconnectionmap_t;

class DeviceManager
{
	public:
		static void refresh();
		static motemap_t motes;	// the current valid motes
		static motemap_t lostMotes; // motes that were just detached
		static motemap_t newMotes; // motes that were just attached
	private:
		static void readUsbDevices();
		static void readMoteDevices();
		static usbdevice_t* findUsbDevice( usbdevice_t* parentdev,
		                                   std::string::const_iterator start,
		                                   std::string::const_iterator end );
		static usbserialmap_t devicesById;
		static usbconnectionmap_t devicesByConnection;
};

}}

#endif
