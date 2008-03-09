#ifndef _SERIALCONTROL_H_
#define _SERIALCONTROL_H_

#include <string>
#include <errno.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "libutil/Log.h"
#include "types.h"

namespace remote { namespace diku_mch {

using namespace util;

class SerialControl
{
	public:
		SerialControl();
		~SerialControl();
		bool runChild(char * const args[], char * const envp[]);
		ssize_t readBuf(char *buf, size_t len);
		ssize_t writeBuf(const char *buf, size_t len);
		int getFd();
	protected:
		bool hasChild();
		bool isOpen();
		bool openTty(const std::string tty);
		void closeTty();
		bool endChild(bool killChild);
		bool controlDTR(bool enable);
		int port;
		pid_t childPid;
		struct termios oldsertio;
};

}}

#endif
