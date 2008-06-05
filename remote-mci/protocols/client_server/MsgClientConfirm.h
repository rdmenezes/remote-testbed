#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_MSGCLIENTCONFIRM_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_MSGCLIENTCONFIRM_H
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgClientRequest.h"
#include "localconstants.h"

namespace remote { namespace protocols { namespace client_server {

class MsgClientConfirm : public BaseMsg
{
	public:
		MsgClientConfirm(uint8_t command, result_t result, dbkey_t mote_id);
		MsgClientConfirm(uint8_t command, result_t result, dbkey_t mote_id, MsgPayload& moteMsg);
		MsgClientConfirm(uint8_t* buffer, uint32_t& buflen);
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint8_t getCommand();
		result_t getResult();
		dbkey_t getMoteId();

		MsgPayload& getMoteMsg();
	protected:
		uint8_t command;
		result_t result;
		dbkey_t mote_id;
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		MsgPayload moteMsg;
};

}}}

#endif
