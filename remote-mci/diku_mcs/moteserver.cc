#include "FileDescriptor.h"
#include "SessionListener.h"
#include "HostListener.h"
#include "database.h"
#include "Configuration.h"
#include "MMSException.h"
#include "macros.h"
#include <exception>
#include <unistd.h>
#include "libutil/File.h"

using namespace remote;
using namespace remote::diku_mcs;
using namespace remote::util;

void handleExit()
{
	log("Shutting down\n");
	remove(Configuration::vm["pidFile"].as<std::string>().c_str());
}

int main(int argc,char** argv)
{
	std::ostringstream oss;
	std::string pid;

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

	atexit(handleExit);

	oss << getpid() << std::endl;
	pid = oss.str();

	if (!File::writeFile(Configuration::vm["pidFile"].as<std::string>(),
			     pid.c_str(), pid.size()))
		log("Failed to create .pid file\n");

	do {
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

	} while (FileDescriptor::serviceLoop());
}
