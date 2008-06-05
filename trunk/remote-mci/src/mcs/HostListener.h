#ifndef REMOTE_MCS_HOSTLISTENER_H
#define REMOTE_MCS_HOSTLISTENER_H

#include "remote.h"

#include "mcs/FileDescriptor.h"
#include "mcs/Host.h"

namespace remote { namespace mcs {

/** This class listen for new host connections and creates
 * host objects for the host connections that are accepted **/
class HostListener : public FileDescriptor
{
	public:
		/** Constructor sets up the host listener
		 * \param port Port on which to accept new host connections
		**/
		HostListener(unsigned int port);
		/** Destructor **/
		virtual ~HostListener();
	private:
		/** Create a new host if it is listed in the database.
		 * \param p_fd File descriptor for the new host connection
		 * \param client Address of the new host
		 * \returns TRUE if the host object was created, FALSE otherwise
		**/
		bool createHostByConnection(int p_fd, sockaddr_in& client);
		/** Handle an event on the file descriptor.
		 * \param events Event descriptor.
		 *  **/
		void handleEvent(short events);
		/** The local Internet socket address for serving hosts. */
		struct sockaddr_in server;
		/** Map of all currently connected hosts. **/
		hostmapbykey_t hosts;
};

}}

#endif
