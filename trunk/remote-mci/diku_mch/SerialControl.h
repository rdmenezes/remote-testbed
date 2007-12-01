#ifndef _SERIALCONTROL_H_
#define _SERIALCONTROL_H_

#include <string>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "macros.h"
#include "types.h"
#include "motecontrol/localconstants.h"

namespace remote { namespace diku_mch {

using namespace protocols;
using namespace protocols::motecontrol;

class SerialControl
{
	public:
		SerialControl();
		~SerialControl();
		result_t openTty();
		result_t closeTty();
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
		const std::string& getTty();
	protected:
		bool hasChild();
		void cleanUpProgram();
		bool clearDTR();
		bool setDTR();
		int port;
		bool isRunning, isOpen;
		pid_t childPid;
		result_t childResult;
		std::string tty;
		struct termios newsertio, oldsertio;
};

}}

#endif
