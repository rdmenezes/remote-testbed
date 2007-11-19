#include "SerialControl.h"

namespace remote { namespace diku_mch {

SerialControl::SerialControl(std::string& p_tty)
	       : isRunning(false), isOpen(false), isProgramming(false), wasProgramming(false)
{
	tty = p_tty;
}

result_t SerialControl::openTty()
{
	log("Opening TTY %s\n", tty.c_str());
	if (isOpen) {
		log("TTY already open for %s\n", tty.c_str());
		return FAILURE;
	}

	port = open(tty.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (port < 0) {
		log("No device connected on %s.\n", tty.c_str());
		return FAILURE;
	}

	tcgetattr(port, &oldsertio); /* save current port settings */

	/*
	 * 8 data, no parity, 1 stop bit. Ignore modem control lines. Enable
	 * receive. Set 38400 baud rate. NO HARDWARE FLOW CONTROL!
	 */
	newsertio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;

	/* Raw input. Ignore errors and breaks. */
	newsertio.c_iflag = IGNBRK | IGNPAR;

	/* Raw output. */
	newsertio.c_oflag = 0;

	/* No echo and no signals. */
	newsertio.c_lflag = 0;

	/* blocking read until 1 char arrives */
	newsertio.c_cc[VMIN]=1;
	newsertio.c_cc[VTIME]=0;

	/* now clean the modem line and activate the settings for modem */
	tcflush(port, TCIFLUSH);
	tcsetattr(port,TCSANOW,&newsertio);

	isOpen = true;
	// open in a stopped state
	if (stop() == SUCCESS )
	{
		return SUCCESS;
	}
	else
	{
		closeTty();
		return FAILURE;
	}
}

result_t SerialControl::closeTty()
{
	log("Closing SerialControl for %s\n", tty.c_str());
	if (!isOpen) {
		log("SerialControl not open for %s\n", tty.c_str());
		return FAILURE;
	}
	stop();
	tcsetattr(port,TCSANOW,&oldsertio);
	close(port);
	isOpen = false;
	return SUCCESS;
}

pid_t SerialControl::program(const std::string& mac, uint16_t tosAddress, std::string program)
{
	std::string tos = getTosStr(tosAddress);
	char * const args[] = {
		(char *) Configuration::vm["moteProgrammerPath"].as<std::string>().c_str(),
		(char *) tty.c_str(),
		"115200",
		(char *) program.c_str(),
		(char *) mac.c_str(),
		(char *) tos.c_str(),
		NULL
	};

	log("Programming TTY %s using mac %s\n", tty.c_str(), mac.c_str());

	prg_pid = run(args);
	if (prg_pid > 0) {
		flashFile = program;
		isProgramming = true;
		prg_result = FAILURE; // no result yet
	}

	return prg_pid;
}

pid_t SerialControl::run(char * const args[])
{
	int pfd[2];
	pid_t pid;

	pipe(pfd);
	closeTty();
	switch (pid = fork())
	{
		case 0:
			/* Redirect output from the child to the
			 * parent's pipe. */
			dup2(pfd[1], STDOUT_FILENO);
			dup2(pfd[1], STDERR_FILENO);
			close(pfd[0]);   // not using pipe read fd
			close(pfd[1]);       // no longer needed- we have a duplicate
			execv(args[0], args);
			fprintf(stderr, "\nFailed to run %s\n", args[0]);
			/* XXX: Make the failed child exit immediately. */
			_exit(EXIT_FAILURE);
			break;
		default:
			close(pfd[1]);     // close the writer pipe end
			// set the port fd to the reader pipe end
			port = pfd[0];
			return pid;
	}

	return -1;
}

bool SerialControl::getProgrammingResult(result_t& result )
{
	result = prg_result;
	bool wasPrg = wasProgramming;
	wasProgramming = false;
	return wasPrg;
}

result_t SerialControl::cancelProgramming()
{
	if (!isProgramming) return FAILURE;
	kill(prg_pid,SIGKILL);
	cleanUpProgram();
	return SUCCESS;
}

void SerialControl::cleanUpProgram()
{
	int status;

	waitpid(prg_pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		prg_result = SUCCESS;
	} else {
		prg_result = FAILURE;
	}
	close(port);
	openTty();
	isProgramming = false;
	wasProgramming = true;
	remove(flashFile.c_str());
	flashFile = "";
}

result_t SerialControl::reset()
{
	if (!isOpen) return FAILURE;
	if (isRunning)
	{
		stop();
		start();
	}
	else
	{
		start();
		stop();
	}
	return SUCCESS;
}

result_t SerialControl::start()
{
	if (!isOpen || !clearDTR()) return FAILURE;
	isRunning = true;
	return SUCCESS;
}

result_t SerialControl::stop()
{
	if ( !isOpen || !setDTR() ) return FAILURE;
	isRunning = false;
	return SUCCESS;
}

bool SerialControl::setDTR()
{
	// reset button down
	int tmp;
	tmp = TIOCM_DTR;
	if (ioctl(port, TIOCMBIS, &tmp) == -1)
	{
		isOpen = false;
		return false;
	}
	return true;
}

bool SerialControl::clearDTR()
{
	// reset button up
	int tmp;
	tmp = TIOCM_DTR;
	if (ioctl(port, TIOCMBIC, &tmp) == -1)
	{
		isOpen = false;
		return false;
	}
	return true;
}

int SerialControl::readBuf(char* buf, int len)
{
	int res;

	if ( (res = read(port,buf,len)) <= 0 && isProgramming)
	{
		cleanUpProgram();
	}
	return res;
}

int SerialControl::writeBuf(const char* buf, int len)
{
	return write(port,buf,len);
}

int SerialControl::getFd()
{
	return port;
}

status_t SerialControl::getStatus()
{
	if (isProgramming) return MOTE_PROGRAMMING;
	if (!isOpen) return MOTE_UNAVAILABLE;
	if (isRunning) return MOTE_RUNNING;
	return MOTE_STOPPED;
}

}}
