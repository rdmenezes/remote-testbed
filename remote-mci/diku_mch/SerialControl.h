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
#include "motecontrol/localconstants.h"

namespace remote { namespace diku_mch {

using namespace protocols;
using namespace protocols::motecontrol;
using namespace util;

class SerialControl
{
	public:
		SerialControl();
		~SerialControl();
		bool runChild(char * const args[], char * const envp[]);
		result_t getChildResult();
		result_t cancelProgramming();
		result_t reset();
		result_t start();
		result_t stop();
		ssize_t readBuf(char *buf, size_t len);
		ssize_t writeBuf(const char *buf, size_t len);
		int getFd();
		status_t getStatus();
	protected:
		bool hasChild();
		bool isOpen();
		bool openTty();
		void closeTty();
		bool endChild(bool killChild);
		result_t power(const std::string cmd);
		bool controlDTR(bool enable);
		int port;
		bool isRunning;
		pid_t childPid;
		std::string tty;
		struct termios oldsertio;
};

}}

#endif
