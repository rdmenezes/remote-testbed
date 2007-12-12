#include "SerialControl.h"

namespace remote { namespace diku_mch {

SerialControl::SerialControl()
	: port(-1), isRunning(false), portIsOpen(false), childPid(-1)
{
}

SerialControl::~SerialControl()
{
	endChild(true);
	if (isOpen())
		closeTty();
}

result_t SerialControl::openTty()
{
	struct termios newsertio;

	if (isOpen()) {
		Log::debug("TTY already open for %s", tty.c_str());
		return FAILURE;
	}

	Log::info("Opening TTY %s", tty.c_str());
	port = open(tty.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (port == -1) {
		Log::error("Failed to open %s: %s", tty.c_str(), strerror(errno));
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

	portIsOpen = true;
	// open in a stopped state
	if (stop() == FAILURE) {
		closeTty();
		return FAILURE;
	}

	return SUCCESS;
}

result_t SerialControl::closeTty()
{
	if (!isOpen()) {
		Log::debug("TTY already closed for %s", tty.c_str());
		return FAILURE;
	}
	Log::info("Closing %s", tty.c_str());
	tcsetattr(port, TCSANOW, &oldsertio);
	close(port);
	port = -1;
	portIsOpen = false;
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

	return hasChild();
}

result_t SerialControl::getChildResult()
{
	return endChild(false) ? SUCCESS : FAILURE;
}

result_t SerialControl::cancelProgramming()
{
	return endChild(true) ? SUCCESS : FAILURE;
}

bool SerialControl::endChild(bool killChild)
{
	int status;

	if (!hasChild())
		return false;

	if (killChild)
		kill(childPid, SIGKILL);

	waitpid(childPid, &status, 0);
	close(port);
	port = childPid = -1;
	openTty();

	return killChild || (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

result_t SerialControl::reset()
{
	if (!isOpen())
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
	if (!isOpen() || !controlDTR(false))
		return FAILURE;
	isRunning = true;
	return SUCCESS;
}

result_t SerialControl::stop()
{
	if (!isOpen() || !controlDTR(true))
		return FAILURE;
	isRunning = false;
	return SUCCESS;
}

bool SerialControl::controlDTR(bool enable)
{
	int tmp = TIOCM_DTR;
	int req = enable ? TIOCMBIS : TIOCMBIC;

	if (ioctl(port, req, &tmp) == -1) {
		Log::error("ioctl(%s) failed: %s", tty.c_str(), strerror(errno));
		portIsOpen = false;
		return false;
	}
	return true;
}

ssize_t SerialControl::readBuf(char *buf, size_t len)
{
	ssize_t res = read(port, buf, len);

	if (res <= 0 && !hasChild())
		closeTty();

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

bool SerialControl::isOpen()
{
	return portIsOpen;
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
	if (!isOpen()) return MOTE_UNAVAILABLE;
	if (isRunning) return MOTE_RUNNING;
	return MOTE_STOPPED;
}

}}
