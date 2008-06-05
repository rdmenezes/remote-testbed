#ifndef REMOTE_PROTOCOLS_HOST_SERVER_MSGHOSTREQUEST_H
#define REMOTE_PROTOCOLS_HOST_SERVER_MSGHOSTREQUEST_H

#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgPayload.h"
#include "MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace host_server {

class MsgHostRequest : public BaseMsg
{
	public:
		MsgHostRequest(MsgMoteAddresses& adressess, MsgPayload& message);
		MsgHostRequest(uint8_t*& buffer, uint32_t& buflen);
		~MsgHostRequest();

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		MsgMoteAddresses& getMoteAddresses();
		MsgPayload& getMessage();

	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		MsgMoteAddresses* addresses;
		MsgPayload* message;
		bool deleteMsg;
};

}}}
#endif
