#ifndef _HOST_H_
#define _HOST_H_

#include "tcputil.h"
#include "types.h"
#include "Mote.h"
#include "FileDescriptor.h"
#include "MoteControlInfrastructure.h"
#include "MoteAddresses.h"
#include "diku_host_server/HostMsg.h"
#include "diku_host_server/MsgPlugEvent.h"
#include "diku_host_server/MsgHostRequest.h"
#include "diku_host_server/MsgHostConfirm.h"
#include "diku_host_server/MsgMoteAddresses.h"
#include "diku_host_server/MsgMoteConnectionInfoList.h"
#include "diku_host_server/MsgMoteConnectionInfo.h"
#include <stdlib.h>

namespace remote { namespace diku_mcs {

using namespace protocols::diku_host_server;
using namespace remote::mcs;

//class Mote;
class Host;

typedef struct moteinfo_s
{
	uint16_t tosAddress;
} moteinfo_t;

typedef std::map<std::string, Mote *> motemapbymac_t;
typedef std::map<std::string, moteinfo_t> moteinfobymac_t;
typedef std::map<dbkey_t,Host*> hostmapbykey_t;

/** This class handles a single mote host connection. It listens for infrastructure changes and
 *  updates the mote database accordingly. It routes mote messages to and from the mote host.
 **/
class Host : public FileDescriptor, public MoteControlInfrastructure
{
	public:
		/** Send a message to a mote.
		 * \param address Address of the mote
		 * \param request Message payload
		**/
		void request( MCIAddress& address, MsgPayload& request );

		/** Constructor - create a new host connection
		 * \param fd File descriptor for this host connection
		 * \param p_id Database key of this host
		 * \param IP address of this host
		 * \param host map to register the new host connection object in
		 **/
		Host( int fd, dbkey_t p_id, std::string ip, hostmapbykey_t& p_hostMap);
		/** Clean up this host and destroy it.
		 * \param silent If true, catch and log any exceptions
		**/
		void destroy(bool silent=false);
	protected:
		/** Destructor **/
		virtual ~Host();
		/** Handle an event that occured on the file descriptor
		 * \param events Event descriptor
		**/
		void handleEvent(short events); // handle an event on the host socket
		/** Handle a message received from a mote host.
		 * \param msgHostMote Message that was received.
		**/
		void handleMsgIn(MsgHostConfirm& msgHostConfirm);
		/** Update the local image of the mote infrastructure to reflect
		 * that a set of motes were unplugged.
		 * \param infolist List of motes that were unplugged
		 **/
		void handleMotesLostList(MsgMoteConnectionInfoList& infolist);
		/** Update the local image of the mote infrastructure to reflect
		 * that a set of motes were plugged in.
		 * \param infolist List of motes that were plugged in
		 **/
		void handleMotesFoundList(MsgMoteConnectionInfoList& infolist);

		/** Register this mote in a map by its MAC address.
		 * \param mac MAC address of the mote.
		 * \param mote Pointer for the mote object.
		**/
		void registerMoteByMac(std::string mac, Mote *mote);

		/** Delete a mote object by its MAC address.
		 * \param mac MAC address of the mote to delete.
		**/
		void deleteMoteByMac(std::string mac);
		/** Register additional mote information by mote MAC address.
		 * \param mac MAC address of the mote
		 * \param moteinfo Additional mote information.
		**/
		void registerMoteInfoByMac(std::string mac, moteinfo_t moteinfo);
		/** Delete additional mote info by mote MAC address.
		 * \param mac MAC address of the mote
		**/
		void deleteMoteInfoByMac(std::string mac);
		/** Check if a mote object exists, and create it if it doesn't.
		 * \param info Connection information of the mote (MAC address and
		 * bus location string.
		 **/
		void findOrCreateMote(MsgMoteConnectionInfo& info);

		/** Database key of this host **/
		dbkey_t id;
		/** Index og this host in the host map **/
		unsigned int hostIndex;
		/** Map of the motes attached to this host keyed by mote MAC **/
		motemapbymac_t motesByMac;
		/** Map of additional deployment sepcific mote information keyed by mote MAC **/
		moteinfobymac_t moteInfoByMac;
		/** Structure for incoming messages **/
		Message message_in;
		/** IP address of this mote host **/
		std::string ipaddress;
		/** Reference to the host map that this host is registered in **/
		hostmapbykey_t& hostMap;
};

}}

#endif
