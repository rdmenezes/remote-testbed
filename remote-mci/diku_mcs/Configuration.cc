#include "Configuration.h"

namespace remote { namespace diku_mcs {
	
void Configuration::read(int ac, char** av )
{
	// command line options
	po::options_description cmdline_options("commandlineoptions");
	cmdline_options.add_options()
    ("config-file",
    	po::value<std::string>()->default_value("/etc/diku_mcs.cfg"), 
	    "Path to the configuration file.")
	("daemonize",
		po::value<int>()->default_value(false), 
		"Run as a daemon.");
    
	// declare all configuration groups
	po::options_description config("Configuration");
	
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
	 ("log-file",
    	po::value<std::string>()->default_value("/var/log/diku_mcs.log"), 
	    "Path to the output log file when running as a daemon.")
 	 ("errorlog-file",
    	po::value<std::string>()->default_value("/var/log/diku_mcs_error.log"), 
	    "Path to the error log file when running as a daemon.")
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
