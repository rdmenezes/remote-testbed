#include "FileDescriptor.h"
#include "macros.h"

namespace remote { namespace mcs {
	
void FileDescriptor::serviceLoop()
{
	unsigned int i, instanceCount;
	int f;
	filedescriptorsbyfd_t::iterator fileDescriptorIterator;
	// register the timeout handler	
	signal( SIGALRM, timeoutHandler );
	ignoreBrokenPipeSignals();
			
	do
	{
		instanceCount = instances.size();
		pollfd fdmap[instanceCount];
		FileDescriptor::buildPollMap(fdmap);
		if ( poll(fdmap,instanceCount,-1) < 0)
		{
			__THROW__("Error in poll!");
		}
		
		try
		{
			// look for activity on any sockets
			for ( i = 0; i < instanceCount; i++ )
			{
				if ( fdmap[i].revents )
				{
					f = fdmap[i].fd;
					fileDescriptorIterator = instances.find(f);
					if (fileDescriptorIterator != instances.end())
					{
						fileDescriptorIterator->second->handleEvent(fdmap[i].revents);
					}
				}
			}
		}
		catch (remote::protocols::MMSException e)
		{
			clearTimeout();
			log("Exception: %s\n",e.what());						
			fileDescriptorIterator->second->clearTimeout();
			fileDescriptorIterator->second->destroy(false);
		}
	} 
	while( true );
}

FileDescriptor::FileDescriptor(int p_fd)
{
	clearTimeout();
	fd = p_fd;
	log("Opened fd %i\n",fd);	
	instances[fd] = this;
}

void FileDescriptor::buildPollMap(pollfd* map)
{
	int j = 0;
	filedescriptorsbyfd_t::iterator i;
	for (i = instances.begin(); i != instances.end(); i++)
	{
		map[j].fd = i->second->fd;
		map[j].events = POLLMSG|POLLRDBAND|POLLRDNORM|POLLIN|POLLPRI|POLLERR|POLLHUP|POLLNVAL;
		map[j].revents = 0;
		j++;
	}
}


FileDescriptor::~FileDescriptor()
{
	if (close(fd) != 0)
	{
		int err = errno;
		switch (err)
		{
			case EBADF:
				log("close(%i) - %i isn’t a valid open file descriptor.\n",fd,fd);
				break;
			case EINTR:
				log("close(%i) - The close() call was interrupted by a signal.\n",fd);
				break;
			case EIO:
				log("close(%i) - An I/O error occurred.\n",fd);
				break;
			default:
				log("close(%i) - Unknown error %i occured\n",fd,err);
		}
	}
	else
	{
		log("Closed fd %i\n",fd);	
	}
	instances.erase(fd);
}

void FileDescriptor::destroy(bool silent)
{
	delete this;
}


void FileDescriptor::setTimeout(int secs)
{
		itimerval timeout;
		// stop the timer
		timeout.it_interval.tv_sec = 0;
		timeout.it_interval.tv_usec = 0;
		timeout.it_value.tv_sec = secs;
		timeout.it_value.tv_usec = 0;
		if (setitimer(ITIMER_REAL, &timeout, NULL))
		{
			__THROW__ ("Timer set error\n");
		};
		currentFd = fd;
}

void FileDescriptor::clearTimeout()
{
		itimerval timeout;
		// stop the timer
		timeout.it_interval.tv_sec = 0;
		timeout.it_interval.tv_usec = 0;
		timeout.it_value.tv_sec = 0;
		timeout.it_value.tv_usec = 0;
		if (setitimer(ITIMER_REAL, &timeout, NULL))
		{
			__THROW__ ("Timer clear error\n");
		};
		currentFd = -1;
}


RETSIGTYPE FileDescriptor::timeoutHandler(int sig)
{
	log( "Timeout occured!\n");
	close(currentFd); // this should generate an exception
}

void FileDescriptor::ignoreBrokenPipeSignals()
{
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	
	sigaction(SIGPIPE,&act,NULL);
}

filedescriptorsbyfd_t FileDescriptor::instances;
int FileDescriptor::currentFd = -1;
}}
