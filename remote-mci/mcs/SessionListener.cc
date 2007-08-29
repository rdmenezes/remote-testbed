#include "SessionListener.h"

namespace remote { namespace mcs {
	
SessionListener::SessionListener(unsigned int port)
              : FileDescriptor(
                openServerSocket( server,
                                  port,
                                  5,
                                  5 )),
                sessions()
{
	log("Listening for client connections on port %u.\n",port);
}

SessionListener::~SessionListener()
{
	sessionmapbyfd_t::iterator cI;
			
	for ( cI = sessions.begin(); cI != sessions.end(); cI++ )
	{
		cI->second->destroy(true);
	}
}

void SessionListener::handleEvent(short events)
{
	// for now, just try to accept a client connection
	struct sockaddr_in client;

	if ( events & POLLIN || events & POLLPRI )
	{
		log("Accepting new client connection.\n");
	
		new Session(nextClient(fd,client),sessions);
	}
	
}

}}
