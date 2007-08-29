#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <boost/program_options.hpp>

namespace remote { namespace diku_mch {

namespace po = boost::program_options;

class Configuration
{
	public:
		static void read( int ac, char** av );
		static po::variables_map vm;
};

}}
#endif /*CONFIGURATION_H_*/
