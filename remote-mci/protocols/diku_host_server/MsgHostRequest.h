#ifndef MSGHOSTREQUEST_H_
#define MSGHOSTREQUEST_H_

#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgPayload.h"
#include "MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace diku_host_server {

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
#endif /*MSGHOSTREQUEST_H_*/
