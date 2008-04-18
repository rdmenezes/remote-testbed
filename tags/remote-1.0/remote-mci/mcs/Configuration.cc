#include "Configuration.h"

namespace remote { namespace mcs {

void Configuration::printHelp(po::options_description& desc)
{
	std::cerr << PACKAGE_NAME << " (MCH) version " << PACKAGE_VERSION
		  << std::endl << std::endl
		  << desc
		  << std::endl;
	exit(0);
}

void Configuration::read(int argc, char **argv)
{
	// command line options
	po::options_description cmdline_options("Command line options");

	cmdline_options.add_options()
	("help", "Print usage help and exit")
	("config-help", "Print config file usage help and exit")
	("config-file",
		 po::value<std::string>()->default_value("/etc/remote-mcs.cfg"),
		 "Path to the configuration file.")
	("daemonize",
		po::value<int>()->default_value(false),
		"Run as a daemon.");

	// declare all configuration groups
	po::options_description config("Configuration options");

	config.add_options()
	("dbName",
		po::value<std::string>(),
		"Name of the infrastructure database.")
	("dbHost",
		po::value<std::string>(),
		"Host name of the infrastructure database server.")
	("dbUser",
		po::value<std::string>(),
		"User name for the infrastructure database.")
	("dbPassword",
		po::value<std::string>(),
		"Password for the infrastructure database.")
	("sessionListenerPort",
		po::value<unsigned int>()->default_value(10000),
		"Port number to use when listening for new sessions.")
	("hostListenerPort",
		po::value<unsigned int>()->default_value(10001),
		"Port number to use when listening for new hosts.")
	("pidFile",
		po::value<std::string>()->default_value("/var/run/remote-mcs.pid"),
		"Path to the file containing the PID of the mote host.")
	("log-file",
		po::value<std::string>()->default_value("/var/log/remote-mcs.log"),
		"Path to the output log file when running as a daemon.")
	("errorlog-file",
		po::value<std::string>()->default_value("/var/log/remote-mcs-error.log"),
		"Path to the error log file when running as a daemon.")
	;

	try {
		store(po::parse_command_line(argc, argv, cmdline_options), vm);
	} catch (boost::program_options::error exception) {
		std::cerr << "Error while parsing command line options: "
			  << exception.what() << std::endl << std::endl;
		printHelp(cmdline_options);
	}
	notify(vm);

	if (vm.count("help"))
		printHelp(cmdline_options);
	if (vm.count("config-help"))
		printHelp(config);

	// get the configuration settings from a file
	std::ifstream ifs(Configuration::vm["config-file"].as<std::string>().c_str());
	try {
		store(parse_config_file(ifs, config), vm);
	} catch (boost::program_options::error exception) {
		std::cerr << "Error while parsing configuration file: "
			  << exception.what() << std::endl << std::endl;
		printHelp(config);
	}
	notify(vm);
}

po::variables_map Configuration::vm;

}}
