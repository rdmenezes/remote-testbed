#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_CLIENTMSG_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_CLIENTMSG_H
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgClientRequest.h"
#include "MsgClientConfirm.h"
#include "MsgSession.h"


namespace remote { namespace protocols { namespace client_server {

enum ClientMsgType
{
	CLIENTMSG_SESSION = 0,
	CLIENTMSG_CLIENTREQUEST = 1,
	CLIENTMSG_CLIENTCONFIRM = 2
};

class ClientMsg : public BaseMsg
{
	static const uint32_t CurrentProtocolVersion = 1;
	public:
		ClientMsg(uint8_t*& buffer, uint32_t& buflen);
		ClientMsg(MsgSession& message);
		ClientMsg(MsgClientRequest& message);
		ClientMsg(MsgClientConfirm& message);
		~ClientMsg();

		uint32_t getLength();

		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint8_t getType();
		uint32_t getProtocolVersion();

		MsgSession& getSession();
		MsgClientRequest& getClientRequest();
		MsgClientConfirm& getClientConfirm();

	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint8_t type;
		uint32_t protocolVersion;
		BaseMsg* message;
		bool deleteMsg;
};

}}}

#endif
