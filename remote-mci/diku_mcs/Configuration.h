#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

namespace remote { namespace diku_mcs {

namespace po = boost::program_options;

/** Configuration handling
 * This class reads options from a configuration file
 * and from the command prompt.
 */
class Configuration
{
	public:
		/** Read options from command line and configuration file
		 * @param argc	Number of command line arguments
		 * @param argv	string array of command line arguments
		 */
		static void read(int argc, char **argv);

		/** Print usage help for option group
		 * @param desc	Option group for which to print help.
		 */
		static void printHelp(po::options_description& desc);

		/** Option variable map keyed by option name */
		static po::variables_map vm;
};

}}

#endif /* CONFIGURATION_H_ */
