#include "Configuration.h"

namespace remote { namespace diku_mch {

void Configuration::read( int ac, char** av )
{
	// command line options
	po::options_description cmdline_options("commandlineoptions");
	cmdline_options.add_options()
	("config-file",
		 po::value<std::string>()->default_value("/etc/diku_mch.cfg"),
		 "Path to the configuration file.")
	("daemonize",
		po::value<int>()->default_value(false),
		"Run as a daemon.");

	// declare all configuration groups
	po::options_description config("Configuration");

	config.add_options()
	("usbserialPath",
		po::value<std::string>()->default_value("/proc/tty/driver/usbserial"),
		"Path to the usbserial device file.")
	("usbdevicePath",
		po::value<std::string>()->default_value("/proc/bus/usb/devices"),
		"Path to the usb system device file.")
	("usbPlugEventPipe",
		po::value<std::string>()->default_value("/var/run/motehost.events"),
		"Path to the fifo notifying the motehost of mote hotplug events.")
	("macPrefix",
		po::value<uint64_t>()->default_value(0x0050C237),
		"The initial part of MAC adresses for DIG528-2 boards.")
	("vendorId",
		po::value<std::string>()->default_value("0050"),
		"USB Vendor ID for the DIG528-2 boards.")
	("productId",
		po::value<std::string>()->default_value("c237"),
		"USB Product ID for the DIG528-2 boards.")
	("usbSerialDevicePath",
		po::value<std::string>()->default_value("/dev/ttyUSB%u"),
		"Path pattern of usb devices.")
	("maxDeviceFileSize",
		po::value<uint16_t>()->default_value(10000),
		"Maximum size in bytes of the device info files.")
	("serverAddress",
		po::value<std::string>(),
		"DNS or IP address of the mote server.")
	("serverPort",
		po::value<uint16_t>()->default_value(10001),
		"Port number to use for connecting to the mote server.")
	("serverConnectionRetryInterval",
		po::value<uint64_t>()->default_value(30),
		"Number of seconds to wait between server connection retries.")
	("moteProgrammerPath",
		po::value<std::string>()->default_value("/usr/bin/hc08sprg"),
		"Path to the DIG528-2 flash programmer.")
	("flashImagePath",
		po::value<std::string>()->default_value("/var/run/motehost_flash%u.s19"),
		"Temporary naming of flash image files.")
	;

	store(po::parse_command_line(ac, av, cmdline_options), vm);
	notify(vm);

	// get the configuration settings from a file
	std::ifstream ifs(Configuration::vm["config-file"].as<std::string>().c_str());
	store(parse_config_file(ifs, config), vm);
	notify(vm);
}

po::variables_map Configuration::vm;

}}
