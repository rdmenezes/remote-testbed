#include "mcs/HostListener.h"

namespace remote { namespace mcs {

HostListener::HostListener(unsigned int port)
	: FileDescriptor(openServerSocket(server, port, 5, 5)), hosts()
{
	Log::info("Listening for host connections on port %u", port);
}

HostListener::~HostListener()
{
	hostmapbykey_t::iterator hI;

	for (hI = hosts.begin(); hI != hosts.end(); hI++)
		hI->second->destroy(true);
}

void HostListener::handleEvent(short events)
{
	struct sockaddr_in client;
	int hostfd;

	if (events & POLLIN || events & POLLPRI) {
		hostfd = nextClient(fd, client);
		if (hostfd >= 0) {
			if (!createHostByConnection(hostfd,client)) {
				Log::info("Host connection denied!");
				close(hostfd);
			} else {
				Log::info("Host connection accepted");
			}
		}
	}
}

bool HostListener::createHostByConnection(int p_fd, sockaddr_in& client)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	// look up the host in the database, get the host id
	std::string ip(inet_ntoa(client.sin_addr));
	mysqlpp::ResUse res;
	mysqlpp::Row row;
	mysqlpp::Query query = sqlConn.query();

	Log::info("Looking up ip %s in host list", ip.c_str());

	query << "select id from host where ip = " << mysqlpp::quote << ip;

	res = query.use();
	res.disable_exceptions();
	row = res.fetch_row();
	if (row && !row.empty()) {
		dbkey_t host_id = (dbkey_t) (row["id"]);
		hostmapbykey_t::iterator i = hosts.find(host_id);
		if (i == hosts.end()) {
			new Host(p_fd, host_id, ip, hosts);
			return true;

		} else {
			// only one connection per host, deny connection
			return false;
		}

	} else {
		// if the host was not found in the database, deny host connection
		return false;
	}
}

}}
