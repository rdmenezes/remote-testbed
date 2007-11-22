#ifndef MSGMOTEADDRESSES_H_
#define MSGMOTEADDRESSES_H_

#include "BaseMsg.h"
#include "MsgPayload.h"

namespace remote { namespace protocols { namespace diku_host_server {

class MsgMoteAddresses : public BaseMsg
{
	public:
		MsgMoteAddresses(std::string mac, uint16_t tosAddress = 0);
		MsgMoteAddresses(uint8_t*& buffer, uint32_t& buflen);
		~MsgMoteAddresses();
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint16_t getTosAddress();
		std::string getMac();
	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);

		uint16_t tosAddress;
		MsgPayload mac;
};

}}}

#endif /*MSGMOTEADDRESSES_H_*/
