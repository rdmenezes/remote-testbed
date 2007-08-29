#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

namespace remote { namespace diku_mcs {

namespace po = boost::program_options;

/** This class reads options from a configuration file
 * and from the command prompt. The Boost Program options
 * library is used to parse the options.
 **/
class Configuration
{
	public:
		/** Read command line options and options from 
		 * configuration file.
		 * \param ac Number of command line arguments
		 * \param av string array of command line arguments
		**/
		static void read( int ac, char** av );
		/** Option variable map keyed by option name **/
		static po::variables_map vm;
};

}}

#endif /*CONFIGURATION_H_*/
