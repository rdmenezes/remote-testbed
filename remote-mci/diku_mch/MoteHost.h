#ifndef REMOTE_MCH_MOTEHOST_H
#define REMOTE_MCH_MOTEHOST_H

#include <string>
#include <sys/select.h>
#include <unistd.h>

#include "macros.h"
#include "types.h"

#include "libutil/error.h"
#include "tcputil.h"
#include "Message.h"
#include "motecontrol/MoteMsg.h"
#include "diku_host_server/HostMsg.h"
#include "diku_host_server/MsgMoteConnectionInfoList.h"
#include "diku_host_server/MsgMoteAddresses.h"
#include "DeviceManager.h"
#include "Mote.h"
#include "Configuration.h"

namespace remote { namespace diku_mch {

using namespace protocols;
using namespace protocols::diku_host_server;
using namespace protocols::motecontrol;

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

	static int clientsock;		/**< Server connection socket. */
	static int plugpipe;		/**< Plug event pipe. */
	static Message msg;		/**< Message manager. */
	static DeviceManager devices;	/**< Mote device manager. */
	static Configuration config;	/**< Option manager. */
};

}}

#endif
