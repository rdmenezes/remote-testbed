#include "DeviceManager.h"

namespace remote { namespace diku_mch {

struct del : public std::unary_function<std::pair<const uint64_t,usbdevice_t*>,void>
{
	void operator()(std::pair<const uint64_t,usbdevice_t*> x) { delete x.second; }
};

struct delMote : public std::unary_function<std::pair<const uint64_t,Mote*>,void>
{
	void operator()(std::pair<const uint64_t,Mote*> x) { delete x.second; }
};

struct invalidate : public std::unary_function<std::pair<const uint64_t,Mote*>,void>
{
	void operator()(std::pair<const uint64_t,Mote*> x)
	{ if (x.second) x.second->invalidate(); }
};

static inline uint64_t
get_device_key(uint8_t dev_bus, uint8_t dev_parent, uint8_t dev_port)
{
	uint64_t key = dev_bus;

	key = key << 8;
	key |= dev_parent;
	key = key << 8;
	key |= dev_port;

	return key;
}

void DeviceManager::refresh()
{
	readUsbDevices();
	readMoteDevices();
}


void DeviceManager::readUsbDevices()
{
	int fd;
	std::string usbdevices,regx,bus,prnt,port,devno,serialno;
	boost::match_flag_type flags;
	std::string::const_iterator start, end;
	usbdevice_t* device;

	// erase any existing info
	std::for_each(devicesByConnection.begin(),devicesByConnection.end(),del());
	devicesById.clear();
	devicesByConnection.clear();

	// read the usb devices file
	char* buffer = new char[Configuration::vm["maxDeviceFileSize"].as<uint16_t>()];
	fd = open(Configuration::vm["usbdevicePath"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
	read(fd,buffer,Configuration::vm["maxDeviceFileSize"].as<uint16_t>()); // non-blocking, so we should have the full contents now
	close(fd);
	usbdevices = buffer;
	delete buffer;
	// create a regular expression to match the desired information per device
	regx = ".*?T: .*?Bus=([0-9]*).*?Prnt=([0-9]*) Port=([0-9]*).*?Dev#=[ ]*([0-9]*).*?(^[ ]*$|SerialNumber=([^\\n]*)).*";
	boost::regex e(regx, boost::regex::perl);
	boost::smatch what;

	flags = boost::match_continuous;
	start = usbdevices.begin();
	end = usbdevices.end();

	// perform the search and register the devices
	while(boost::regex_search(start,end, what, e, flags))
	{
		start = what[1].second;

		devno = what[4].str();
		bus = what[1].str();
		prnt = what[2].str();
		port = what[3].str();
		serialno = what[6].matched ? what[6].str() : "";

		device = new usbdevice_t;
		device->devNo = atoi(devno.c_str());
		device->bus = atoi(bus.c_str());
		device->parent = atoi(prnt.c_str());
		device->port = atoi(port.c_str());
		device->serial = serialno;

		/*printf("Registering usb device bus %s, parent %s, port %s, dev# %s, serial %s\n",
		       bus.c_str(),prnt.c_str(),port.c_str(),devno.c_str(),serialno.c_str());*/
		// register the device in the search structures.
		devicesById[device->serial] = device;
		uint64_t key = get_device_key(device->bus, device->parent,
					      device->port);
		devicesByConnection[key] = device;

		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}
}

void DeviceManager::readMoteDevices()
{
	std::string usbserial,regx,tty,hcid,ports,path;
	std::string::const_iterator start, end;
	boost::match_flag_type flags;
	int fd;
	char *buffer = new char[Configuration::vm["maxDeviceFileSize"].as<uint16_t>()];

	// invalidate any existing motes
	std::for_each(motes.begin(),motes.end(),invalidate());
	// clear new motes and delete and clear lost motes
	newMotes.clear();
	std::for_each(lostMotes.begin(),lostMotes.end(),delMote());
	lostMotes.clear();
	// open the file describing connected usbserial devices
	fd = open(Configuration::vm["usbserialPath"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
	read(fd,buffer,Configuration::vm["maxDeviceFileSize"].as<uint16_t>()); // non-blocking, so we should have the file now
	close(fd);
	usbserial = buffer;

	delete buffer;

	regx = ".*?([0-9]): .*?vendor:";
	regx += Configuration::vm["vendorId"].as<std::string>();
	regx += " .*?product:";
	regx += Configuration::vm["productId"].as<std::string>();
	regx += ".*?path:(usb-(([0-9]|[a-f]|\\.|:)*)-(([0-9]|\\.)*)[^\\n]*).*";

	boost::regex e(regx, boost::regex::perl);
	boost::smatch what;

	flags = boost::match_continuous;

	start = usbserial.begin();
	end = usbserial.end();

	while(boost::regex_search(start,end, what, e, flags))
	{
		start = what[1].second;

		tty = what[1].matched ? what[1].str() : "";
		hcid = what[3].matched ? what[3].str() : "";
		ports = what[5].matched ? what[5].str() : "";
		path = what[2].matched ? what[2].str() : "";
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;

		// now, we did find a device with the right vendor and product id
		// - look up the host controller
		// and use this to traverse to the corresponding usb device entry

		usbserialmap_t::const_iterator r = devicesById.find(hcid);

		if (r == devicesById.end()) __THROW__ ("Go away! You don't exist!\n");

		usbdevice_t* rootDev = r->second;

		usbdevice_t* moteDev = findUsbDevice(rootDev,ports.begin(),ports.end());
		if (moteDev)
		{
			uint64_t mac = Configuration::vm["macPrefix"].as<uint64_t>();
			mac = mac << 32;
			mac |= strtoll(moteDev->serial.c_str(),NULL,16);
			// Time to update the mote collection
			motemap_t::iterator m = motes.find(mac);
			if (m != motes.end())
			{
				// we found a mote with the same MAC address
				// just make sure the info is up-to-date
				m->second->validate( path, atoi(tty.c_str()) );
			}
			else
			{
				Mote* mote = new Mote( mac, path, atoi(tty.c_str()) );
				if (mote->_open() == SUCCESS)
				{
					// this must be a new mote, add it to the collection
					motemap_t::value_type elm(mac,mote);
					motes.insert(elm);
					newMotes.insert(elm);
				}
				else
				{
					delete mote;
				}
			}
		}
	}

	// finally, clean up the motes that were lost since the last refresh
	motemap_t::iterator moteI = motes.begin();
	while (moteI != DeviceManager::motes.end())
	{
		if (moteI->second)
		{
			if  (!moteI->second->isValid())
			{
				moteI->second->_close();
				lostMotes.insert(*moteI);
				motes.erase(moteI);
			}
		}
		else
		{
			motes.erase(moteI);
		}
		moteI++;
	}
}

usbdevice_t* DeviceManager::findUsbDevice(usbdevice_t* dev,
					  std::string::const_iterator start,
					  std::string::const_iterator end )
{
	// get the first port of path
	std::string port, regx;
	boost::match_flag_type flags;
	regx = ".*?([0-9])+\\.*.*";
	boost::regex e(regx, boost::regex::perl);
	boost::smatch what;


	flags = boost::match_continuous;
	if (boost::regex_search(start,end, what, e, flags))
	{
		// subtract 1 from port because usbserial indexes ports starting with 1 instead of 0!!!
		uint64_t key = get_device_key(dev->bus, dev->devNo,
					      atoi(what[1].str().c_str()) - 1);
		usbconnectionmap_t::const_iterator devI = devicesByConnection.find(key);
		if (devI == devicesByConnection.end()) return NULL; // device does not exist
		usbdevice_t* nextdev = devI->second;
		return findUsbDevice(nextdev,what[1].second,end);
	}
	else
	{
		return dev;
	}
}

motemap_t DeviceManager::motes;
motemap_t DeviceManager::newMotes;
motemap_t DeviceManager::lostMotes;

usbserialmap_t DeviceManager::devicesById;
usbconnectionmap_t DeviceManager::devicesByConnection;

}}
