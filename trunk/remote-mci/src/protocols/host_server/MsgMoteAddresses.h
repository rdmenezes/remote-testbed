#ifndef REMOTE_PROTOCOLS_HOST_SERVER_MSGMOTEADDRESSES_H
#define REMOTE_PROTOCOLS_HOST_SERVER_MSGMOTEADDRESSES_H

#include "protocols/BaseMsg.h"
#include "protocols/MsgPayload.h"

namespace remote { namespace protocols { namespace host_server {

class MsgMoteAddresses : public BaseMsg
{
	public:
		MsgMoteAddresses(std::string mac, std::string netAddress = "");
		MsgMoteAddresses(uint8_t*& buffer, uint32_t& buflen);
		~MsgMoteAddresses();
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		std::string getNetAddress();
		std::string getMac();
	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);

		MsgPayload netAddress;
		MsgPayload mac;
};

}}}

#endif
