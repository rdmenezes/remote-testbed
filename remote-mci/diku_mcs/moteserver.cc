#include "FileDescriptor.h"
#include "SessionListener.h"
#include "HostListener.h"
#include "database.h"
#include "Configuration.h"
#include "MMSException.h"
#include "macros.h"
#include <exception>
#include <unistd.h>

namespace mms = remote;
using namespace mms;
using namespace mms::diku_mcs;

int main(int argc,char** argv)
{
	Configuration::read(argc,argv);
	if (Configuration::vm["daemonize"].as<int>())
	{
		printf("Daemonizing!\n");
		if (fork()) exit(0);
		setsid();
		fclose(stdin);		
		close(1);
		// reopen stdout
		if (NULL == freopen(Configuration::vm["log-file"].as<std::string>().c_str(),"a",stdout))
		{
			fprintf(stderr,"Unable to open file %s for logging! Bailing out!\n",Configuration::vm["log-file"].as<std::string>().c_str());
			return -1;
		}
		if (NULL == freopen(Configuration::vm["errorlog-file"].as<std::string>().c_str(),"a",stderr))
		{
			return -1;
		}
	}
	
	log("Starting mote server\n");
	
	while (1)
	{
//		try
//		{
			dbConn.connect( Configuration::vm["dbName"].as<std::string>(),
			                Configuration::vm["dbHost"].as<std::string>(),
			                Configuration::vm["dbUser"].as<std::string>(),
			                Configuration::vm["dbPassword"].as<std::string>());
			log("Connected to database\n");
			Mote::resetDb();
			log("Deleted old mote data\n");
			Session::resetDb();
			log("Deleted old session data\n");
			HostListener hostListener(Configuration::vm["hostListenerPort"].as<unsigned int>());
			SessionListener sessionListener(Configuration::vm["sessionListenerPort"].as<unsigned int>());	
			log("Entering service loop\n");
			FileDescriptor::serviceLoop();
/*		}
		catch (std::exception e)
		{
			log("Caught exception: %s - restarting listeners in 30 seconds\n",e.what());
			usleep(30000000);
		}*/
	}
}