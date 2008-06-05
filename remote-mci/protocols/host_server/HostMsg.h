#ifndef REMOTE_PROTOCOLS_HOST_SERVER_HOSTMSG_H
#define REMOTE_PROTOCOLS_HOST_SERVER_HOSTMSG_H
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgPayload.h"
#include "MsgPlugEvent.h"
#include "MsgHostRequest.h"
#include "MsgHostConfirm.h"

namespace remote { namespace protocols { namespace host_server {

enum HostMsgType
{
	HOSTMSGTYPE_PLUGEVENT,
	HOSTMSGTYPE_HOSTREQUEST,
	HOSTMSGTYPE_HOSTCONFIRM
};

#define HOST_SERVER_PROTOCOL_VERSION  0x00010000

class HostMsg : public BaseMsg
{
	public:
		HostMsg(MsgPlugEvent& message);
		HostMsg(MsgHostRequest& message);
		HostMsg(MsgHostConfirm& message);
		HostMsg(uint8_t*& buffer, uint32_t& buflen);
		~HostMsg();

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint32_t getProtocolVersion();

		uint8_t getType();
		MsgHostRequest& getHostRequest();
		MsgHostConfirm& getHostConfirm();
		MsgPlugEvent& getPlugEvent();

	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint32_t protocolVersion;
		uint8_t type;
		BaseMsg* message;
		bool deleteMsg;
};

}}}

#endif
