#ifndef _MOTEHOST_H_
#define _MOTEHOST_H_

#include <string>
#include <sys/select.h>
#include <unistd.h>

#include "macros.h"
#include "types.h"

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

class MoteHost
{
	public:
		static int main(int argc,char** argv);
	private:
		static void lookForServer();
		static bool makeMoteInfoList(motemap_t& motelist, MsgMoteConnectionInfoList& infolist);
		static void serviceLoop();
		static void handlePlugEvent();
		static void handleMessage();
		static void handleRequest(Mote* mote,MsgMoteAddresses& addresses, MsgRequest& request);
		static void handleMoteData(Mote* p_mote);
		static int rebuildFdSet(fd_set& fdset);
		static bool program(Mote* p_mote, uint16_t tosAddress, MsgPayload& image);
		static int clientsock; // port for the server connection
		static int plugpipe; // pipe for plug events
		static fd_set fdset;
		static Message msg;
		static DeviceManager devices;
};

}}

#endif
