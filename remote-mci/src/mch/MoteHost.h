#ifndef REMOTE_MCH_MOTEHOST_H
#define REMOTE_MCH_MOTEHOST_H

#include "remote.h"

#include "protocols/tcputil.h"
#include "protocols/Message.h"
#include "protocols/motecontrol/MoteMsg.h"
#include "protocols/host_server/HostMsg.h"
#include "protocols/host_server/MsgMoteConnectionInfoList.h"
#include "protocols/host_server/MsgMoteAddresses.h"

#include "mch/DeviceManager.h"
#include "mch/Mote.h"

namespace remote { namespace mch {

using namespace remote::protocols;
using namespace remote::protocols::host_server;
using namespace remote::protocols::motecontrol;

/** Mote control host.
 *
 * This class connects the various MCH parts into a mote host.
 * On startup it will connect to the mote control server and
 * start listening for new control message. It will also open
 * the mote plug pipe to receive events from external scripts
 * when new motes are attached and detached. Finally, it will
 * listen to the TTY of all motes and forward mote console data
 * to the mote control server.
 */
class MoteHost
{
public:
	/** Start mote control host.
	 *
	 * @param argc	Number of command line arguments.
	 * @param argv	String array of command line arguments.
	 * @return	The result code of the terminating mote host.
	 */
	static int main(int argc, char **argv);

private:
	/** Establish connection to server. */
	static void lookForServer();

	/** Main event loop. */
	static void serviceLoop();

	/** Handle mote plug/unplug event. */
	static void handlePlugEvent();

	/** Handle incoming message from mote control server. */
	static void handleMessage();

	/** Handle command request for mote.
	 *
	 * @param mote		The mote to which the request belongs.
	 * @param addresses	The address information of the mote.
	 * @param request	The request to handle.
	 */
	static void handleRequest(Mote *mote, MsgMoteAddresses& addresses, MsgRequest& request);

	/** Handle serial data for mote.
	 *
	 * @param mote		The mote which has incoming serial data.
	 */
	static void handleMoteData(Mote *mote);

	/** Send request confirmation message.
	 *
	 * @param mote		The mote for which the request was addressed.
	 * @param command	The request command that was executed.
	 * @param result	The result of the command execution.
	 */
	static void confirmRequest(Mote *mote, uint8_t command, result_t result);

	/** Get the status code of a mote.
	 *
	 * @param mote		The mote for which to get the status code.
	 * @return		The mote status code.
	 */
	static status_t getMoteStatus(Mote *mote);

	/** Prepare plug event message payload.
	 *
	 * @param motelist	The mote map from which prepare the message.
	 * @param info		The message payload which to prepare.
	 * @return		True if the payload should be sent.
	 */
	static bool makeMoteInfoList(motemap_t& motelist, MsgMoteConnectionInfoList& infolist);

	/** Rebuild select() file descriptor set.
	 *
	 * @param fdset		The file descriptor set to rebuild.
	 * @return		The number of file descriptors in the fdset.
	 */
	static int rebuildFdSet(fd_set& fdset);

	/** Exit handler to be used with atexit.
	 */
	static void handleExit();

	/** Handle received signal
	 *
	 * @param signo		The number of the received signal.
	 */
	static void handleSignal(int signo);

	/** Install signal handlers
	 *
	 * Setup handlers to receive exit signals.
	 */
	static void installSignalHandlers();

	/** Shutdown the mote host?
	 *
	 * @return		True if the mote host should shutdown.
	 */
	static inline bool exitDaemon()
	{
		return exitSignal != 0;
	}

	static int clientsock;		/**< Server connection socket. */
	static int plugpipe;		/**< Plug event pipe. */
	static Message msg;		/**< Message manager. */
	static DeviceManager devices;	/**< Mote device manager. */
	static int exitSignal;		/**< Caught exit signal or zero. */

	/* Option values. */
	static bool daemonize;
	static std::string pidFile;
	static std::string serverHost;
	static uint16_t serverPort;
	static uint64_t retryInterval;
	static std::string eventPipe;
	static std::string devicePath;
};

}}

#endif
