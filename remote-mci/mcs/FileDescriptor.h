#ifndef _FILEDESCRIPTOR_H_
#define _FILEDESCRIPTOR_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "types.h"
#include "database.h"
#include <stdlib.h>
#include <sys/poll.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include "libutil/Log.h"

namespace remote { namespace mcs {

using namespace remote::util;

class FileDescriptor;

typedef std::map<int,FileDescriptor*> filedescriptorsbyfd_t;

/** Base class for handling socket connections through file descriptors.
 *
 **/
class FileDescriptor
{
	public:
		/** Main service loop using poll() system call to receive fd events**/
		static bool serviceLoop();
		/** Handles an event on a specific file descriptor **/
		virtual void handleEvent(short events) = 0;
		/** Clean up and destroy a FileDescriptor object **/
		virtual void destroy(bool silent=false);
	protected:
		void setTimeout(int secs);
		static void clearTimeout();
		FileDescriptor(int p_fd);
		virtual ~FileDescriptor();
		static void buildPollMap(pollfd* map);
		static void installSignalHandlers();
		static void exitSignalHandler(int signo);
		static RETSIGTYPE timeoutHandler(int sig);
		static filedescriptorsbyfd_t instances;
		static int exitSignal;
		static int currentFd;
		int fd;

};

}}

#endif
