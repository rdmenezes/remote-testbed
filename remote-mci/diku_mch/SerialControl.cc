#include "SerialControl.h"

namespace remote { namespace diku_mch {

SerialControl::SerialControl()
	: isRunning(false), isOpen(false), childPid(-1), childResult(NOT_SUPPORTED)
{
}

SerialControl::~SerialControl()
{
	cancelProgramming();
	if (isOpen)
		closeTty();
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
	tcsetattr(port, TCSANOW, &newsertio);

	isOpen = true;
	// open in a stopped state
	if (stop() == SUCCESS) {
		return SUCCESS;
	} else {
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
	tcsetattr(port, TCSANOW, &oldsertio);
	close(port);
	isOpen = false;
	return SUCCESS;
}

bool SerialControl::runChild(char * const args[], char * const envp[])
{
	int pfd[2];

	if (pipe(pfd) < 0)
		return false;
	closeTty();
	if ((childPid = fork())) {
		/* Only use the reader end if we forked. */
		if (hasChild())
			port = pfd[0];
		else
			close(pfd[0]);
		close(pfd[1]);

	} else {
		/* Redirect all standard output to the parent's pipe. */
		if (dup2(pfd[1], STDOUT_FILENO) != -1 &&
		    dup2(pfd[1], STDERR_FILENO) != -1) {
			close(pfd[0]);
			close(pfd[1]);
			execve(args[0], args, envp);
		}
		/* XXX: Make the failed child exit immediately. */
		_exit(EXIT_FAILURE);
	}

	childResult = NOT_SUPPORTED;
	return hasChild();
}

result_t SerialControl::getChildResult()
{
	result_t result = childResult;

	childResult = NOT_SUPPORTED;
	return result;
}

result_t SerialControl::cancelProgramming()
{
	if (!hasChild())
		return FAILURE;
	kill(childPid, SIGKILL);
	cleanUpProgram();
	return SUCCESS;
}

void SerialControl::cleanUpProgram()
{
	int status;

	waitpid(childPid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		childResult = SUCCESS;
	} else {
		childResult = FAILURE;
	}
	childPid = -1;
	close(port);
	openTty();
}

result_t SerialControl::reset()
{
	if (!isOpen)
		return FAILURE;
	if (isRunning) {
		stop();
		start();
	} else {
		start();
		stop();
	}
	return SUCCESS;
}

result_t SerialControl::start()
{
	if (!isOpen || !clearDTR())
		return FAILURE;
	isRunning = true;
	return SUCCESS;
}

result_t SerialControl::stop()
{
	if (!isOpen || !setDTR())
		return FAILURE;
	isRunning = false;
	return SUCCESS;
}

bool SerialControl::setDTR()
{
	int tmp = TIOCM_DTR;

	if (ioctl(port, TIOCMBIS, &tmp) == -1) {
		isOpen = false;
		return false;
	}
	return true;
}

bool SerialControl::clearDTR()
{
	int tmp = TIOCM_DTR;

	if (ioctl(port, TIOCMBIC, &tmp) == -1) {
		isOpen = false;
		return false;
	}
	return true;
}

ssize_t SerialControl::readBuf(char *buf, size_t len)
{
	ssize_t res = read(port, buf, len);

	if (res <= 0 && hasChild()) {
		cleanUpProgram();
	}
	return res;
}

ssize_t SerialControl::writeBuf(const char* buf, size_t len)
{
	return write(port, buf, len);
}

bool SerialControl::hasChild()
{
	return childPid != -1;
}

const std::string& SerialControl::getTty()
{
	return tty;
}

int SerialControl::getFd()
{
	return port;
}

status_t SerialControl::getStatus()
{
	if (hasChild()) return MOTE_PROGRAMMING;
	if (!isOpen) return MOTE_UNAVAILABLE;
	if (isRunning) return MOTE_RUNNING;
	return MOTE_STOPPED;
}

}}
