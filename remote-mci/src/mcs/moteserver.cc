#include "remote.h"

#include "mcs/FileDescriptor.h"
#include "mcs/HostListener.h"
#include "mcs/SessionListener.h"
#include "mcs/database.h"

using namespace remote;
using namespace remote::mcs;

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
	Log::info("Shutting down");
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
		Log::open("remote-mcs", Log::INFO, Log::SYSLOG);
	} else {
		Log::open("remote-mcs", Log::INFO, stdout);
	}

	Log::info("Starting mote server");

	atexit(handleExit);

	oss << getpid() << std::endl;
	pid = oss.str();

	if (!File::writeFile(pidFile, pid.c_str(), pid.size()))
		Log::error("Failed to create .pid file");

	do {
			dbConn.connect(dbName, dbHost, dbUser, dbPassword);
			Log::info("Connected to database");
			Mote::resetDb();
			Log::info("Deleted old mote data");
			Session::resetDb();
			Log::info("Deleted old session data");
			HostListener hostListener(moteHostPort);
			SessionListener sessionListener(sessionPort);
			Log::info("Entering service loop");
			eCode = FileDescriptor::serviceLoop();
	} while (eCode);
}
