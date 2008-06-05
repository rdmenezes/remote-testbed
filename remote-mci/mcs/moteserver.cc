#include "FileDescriptor.h"
#include "SessionListener.h"
#include "HostListener.h"
#include "database.h"
#include "MMSException.h"
#include "macros.h"
#include <exception>
#include <unistd.h>
#include "libutil/File.h"
#include "libutil/Config.h"

using namespace remote;
using namespace remote::mcs;
using namespace remote::util;

std::string dbName	= "REMOTE";
std::string dbHost	= "localhost";
std::string dbUser	= "remote_admin";
std::string dbPassword	= "remote";
std::string logFile	= "/var/log/remote-mcs.log";
std::string errorFile	= "/var/log/remote-mcs-error.log";
std::string pidFile	= "/var/run/remote-mcs.pid";
uint16_t sessionPort	= 10000;
uint16_t moteHostPort	= 10001;
bool daemonize		= false;

void handleExit()
{
	log("Shutting down\n");
	remove(pidFile.c_str());
}

int main(int argc,char** argv)
{
	std::ostringstream oss;
	std::string pid;
	bool eCode = true;
	Config config("remote-mcs", "/etc/remote-mcs.cfg");

	config("daemonize", &daemonize, "Run as daemon");
	config("pidFile", &pidFile, "Path to PID file");
	config("dbName", &dbName, "Database user name");
	config("dbHost", &dbHost, "Database host name");
	config("dbUser", &dbUser, "Database user name");
	config("dbPassword", &dbPassword, "Database password");
	config("sessionPort", &sessionPort, "Port number for sessions");
	config("moteHostPort", &moteHostPort, "Port number for mote hosts");
	config("logFile", &logFile, "Path to log file when running as a daemon");
	config("errorFile", &errorFile, "Path to error file when running as a daemon");

	config("sessionListenerPort", "sessionPort");
	config("hostListenerPort", "moteHostPort");
	config("log-file", "logFile");
	config("errorlog-file", "errorFile");

	if (!config.read(argc, argv))
		return EXIT_FAILURE;

	if (daemonize) {
		printf("Daemonizing!\n");
		if (fork()) exit(0);
		setsid();
		fclose(stdin);
		close(1);
		// reopen stdout
		if (!freopen(logFile.c_str(), "a", stdout)) {
			fprintf(stderr, "Unable to open file %s for logging! Bailing out!\n", logFile.c_str());
			return -1;
		}
		if (!freopen(errorFile.c_str(), "a", stderr)) {
			return -1;
		}
	}

	log("Starting mote server\n");

	atexit(handleExit);

	oss << getpid() << std::endl;
	pid = oss.str();

	if (!File::writeFile(pidFile, pid.c_str(), pid.size()))
		log("Failed to create .pid file\n");

	do {
			dbConn.connect(dbName, dbHost, dbUser, dbPassword);
			log("Connected to database\n");
			Mote::resetDb();
			log("Deleted old mote data\n");
			Session::resetDb();
			log("Deleted old session data\n");
			HostListener hostListener(moteHostPort);
			SessionListener sessionListener(sessionPort);
			log("Entering service loop\n");
			eCode = FileDescriptor::serviceLoop();
	} while (eCode);
}
