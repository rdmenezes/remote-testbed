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

#include "types.h"
#include "motecontrol/localconstants.h"
#include "Configuration.h"

namespace remote { namespace diku_mch {

using namespace protocols;
using namespace protocols::motecontrol;

class SerialControl
{
	public:
		SerialControl(unsigned int portnumber);
		const std::string& getDeviceName();
		result_t _open();
		result_t _close();
		pid_t program(uint64_t macAddress, uint16_t tosAddress,std::string program);
		bool getProgrammingResult(result_t& result);
		result_t cancelProgramming();
		result_t reset();
		result_t start();
		result_t stop();
		int readBuf(char* buf, int len);
		int writeBuf(const char* buf, int len);
		char readChar();
		void writeChar(char c);
		int getFd();
		status_t getStatus();
	protected:
		void cleanUpProgram();
		bool clearDTR();
		bool setDTR();
		int port,prg_pid;
		bool isRunning, isOpen, isProgramming,wasProgramming;
		result_t prg_result;
		std::string DeviceName;
		std::string flashFile;
		struct termios newsertio, oldsertio;
};

}}

#endif
