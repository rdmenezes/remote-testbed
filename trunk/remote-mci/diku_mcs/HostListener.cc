#include "HostListener.h"

namespace remote { namespace diku_mcs {

HostListener::HostListener(unsigned int port)
             : FileDescriptor(
               openServerSocket( server,
                                 port,
                                 5,
                                 5 )),
               hosts()
{
	log("Listening for host connections on port %u.\n",port);
}

HostListener::~HostListener()
{
	deleteAllHosts();
}

void HostListener::handleEvent(short events)
{
	struct sockaddr_in client;
	int hostfd;

	if (events & POLLIN || events & POLLPRI) {
		hostfd = nextClient(fd, client);
		if (hostfd >= 0) {
			log("New host connection\n");
			if (!createHostByConnection(hostfd,client)) {
				log("Host connection denied!\n");
				close(hostfd);
			}
		}
	}
}

bool HostListener::createHostByConnection(int p_fd, sockaddr_in& client)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	// look up the host in the database, get the host id
	std::string ip(inet_ntoa(client.sin_addr));
	log("Looking up ip %s in host list\n", ip.c_str());
	mysqlpp::ResUse res;
	mysqlpp::Row row;
	mysqlpp::Query query = sqlConn.query();

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

void HostListener::deleteAllHosts()
{
	hostmapbykey_t::iterator hI;

	for (hI = hosts.begin(); hI != hosts.end(); hI++) {
		hI->second->destroy(true);
	}
}

}}
