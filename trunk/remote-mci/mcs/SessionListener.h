#ifndef REMOTE_MCS_SESSIONLISTENER_H
#define REMOTE_MCS_SESSIONLISTENER_H

#include "remote.h"

#include "mcs/FileDescriptor.h"
#include "mcs/Session.h"

namespace remote { namespace mcs {

/** This class is responsible for opening a TCP socket and listening
 * for new client connections (sessions).
**/
class SessionListener : public FileDescriptor
{
	public:
		/** Creates and starts a new SessionListener instance.
		 * \param port Port to use when listening for new client connections.
		**/
		SessionListener(unsigned int port);
		/** Destructor **/
		virtual ~SessionListener();
	private:
		/** Handles a new event on the current socket
		 * \param events Event descriptor.
		**/
		void handleEvent(short events);
		/** Server socket **/
		struct sockaddr_in server;
		/** Map of currently active sessions **/
		sessionmapbyfd_t sessions;
};
}}

#endif
