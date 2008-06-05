#ifndef REMOTE_MCH_SERIALCONTROL_H_
#define REMOTE_MCH_SERIALCONTROL_H_

#include "remote.h"

namespace remote { namespace mch {

using namespace util;

/** Serial Connection Control
 *
 * This class provides a low-level interface for managing a serial
 * connection to a mote, such as opening and closing TTYs as well
 * as starting and ending child processes operating on the mote TTYs.
 */
class SerialControl
{
public:
	/** Create serial control. */
	SerialControl();

	/** Destroy serial control. */
	~SerialControl();

	/** Create and run child process.
	 *
	 * @param args		The program and args.
	 * @param envp		The process environment.
	 * @return		True if the start was created.
	 */
	bool runChild(char * const args[], char * const envp[]);

	/** Read buffer from the serial port.
	 *
	 * @param buf		The buffer start position.
	 * @param len		The buffer maximum length.
	 * @return		The number of read bytes.
	 */
	ssize_t readBuf(char *buf, size_t len);

	/** Write buffer to the serial port.
	 *
	 * @param buf		The buffer start position.
	 * @param len		The buffer length.
	 * @return		The number of written bytes.
	 */
	ssize_t writeBuf(const char *buf, size_t len);

	/** Get the file descriptor for the serial port.
	 *
	 * @return		The serial port being controlled.
	 */
	int getFd();

	/** Is the serial control port open?
	 *
	 * @return		True if the port is open.
	 */
	bool isOpen();

protected:
	/** Does any children exist?
	 *
	 * @return		True if the mote has a child.
	 */
	bool hasChild();

	/** Open serial control for TTY
	 *
	 * This method opens the request TTY. The platform string is used
	 * for determining the proper baud rate.
	 *
	 * @param platform	The platform of the mote.
	 * @param tty		The path to the TTY device.
	 * @return		True if the TTY was successfully opened.
	 */
	bool openTty(const std::string platform, const std::string tty);

	/** Close serial control for opened TTY. */
	void closeTty();

	/** Get exit result of child.
	 *
	 * @param killChild	Force the child to end by killing it.
	 * @return		True if the child exited successfully.
	 */
	bool endChild(bool killChild);

	/** Enable or disable DTR.
	 *
	 * The DTR channel is wired to the RESET pin on the dig528-2 motes.
	 * Using this hardware configuration motes can be stopped and started
	 * by enabling/disabling it respectively.
	 *
	 * @param enable	Set the DTR pin high.
	 * @return		True if the operation succeeded.
	 */
	bool controlDTR(bool enable);

	int port;			/**< Serial file descriptor (or -1). */
	pid_t childPid;			/**< Process ID of child (or -1). */
	struct termios oldsertio;	/**< Saved terminal settings. */
};

}}

#endif
