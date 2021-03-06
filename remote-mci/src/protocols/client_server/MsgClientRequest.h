#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_MSGCLIENTREQUEST_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_MSGCLIENTREQUEST_H

#include "protocols/BaseMsg.h"
#include "protocols/MsgPayload.h"
#include "protocols/client_server/MsgMoteIdList.h"

namespace remote { namespace protocols { namespace client_server {

enum MsgClientCommand
{
	MSGCLIENTCOMMAND_GETMOTECONTROL,
	MSGCLIENTCOMMAND_DROPMOTECONTROL,
	MSGCLIENTCOMMAND_MOTEMESSAGE = 100
};

class MsgClientRequest : public BaseMsg
{
	public:
		MsgClientRequest(uint8_t command);
		MsgClientRequest(uint8_t*& buffer, uint32_t& buflen);
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);
		uint8_t getCommand();
		MsgMoteIdList& getMoteIdList();
		MsgPayload& getMoteMsg();
	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint8_t command;
		MsgMoteIdList msgMoteIdList;
		MsgPayload moteMsg;

};

}}}

#endif
