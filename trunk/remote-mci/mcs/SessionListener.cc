#include "SessionListener.h"

namespace remote { namespace mcs {

SessionListener::SessionListener(unsigned int port)
	: FileDescriptor(openServerSocket(server, port, 5, 5)), sessions()
{
	Log::info("Listening for client connections on port %u\n", port);
}

SessionListener::~SessionListener()
{
	sessionmapbyfd_t::iterator cI;

	for (cI = sessions.begin(); cI != sessions.end(); cI++)
		cI->second->destroy(true);
}

void SessionListener::handleEvent(short events)
{
	// for now, just try to accept a client connection
	struct sockaddr_in client;

	if (events & POLLIN || events & POLLPRI) {
		int clientsock = nextClient(fd, client);

		if (clientsock >= 0) {
			Log::info("Accepting new client connection");
			new Session(clientsock, sessions);
		}
	}
}

}}
