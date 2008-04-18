#ifndef REMOTE_MCH_CONFIGURATION_H
#define REMOTE_MCH_CONFIGURATION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

namespace remote { namespace mch {

namespace po = boost::program_options;

/** Configuration handling.
 *
 * This class reads options from a configuration file and from the
 * command line.
 */
class Configuration
{
public:
	/** Read options from command line and configuration file.
	 *
	 * @param argc	Number of command line arguments.
	 * @param argv	String array of command line arguments.
	 */
	void read(int argc, char **argv);

	/** Print usage help for option group.
	 *
	 * @param desc	Option group for which to print help.
	 */
	void printHelp(po::options_description& desc);

	/** Option variable map keyed by option name */
	po::variables_map vm;
};

}}

#endif
