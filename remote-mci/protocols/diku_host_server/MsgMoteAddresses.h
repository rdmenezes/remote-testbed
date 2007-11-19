#ifndef MSGMOTEADDRESSES_H_
#define MSGMOTEADDRESSES_H_

#include "BaseMsg.h"

namespace remote { namespace protocols { namespace diku_host_server {

class MsgMoteAddresses : public BaseMsg
{
	public:
		MsgMoteAddresses(uint16_t tosAddress, uint64_t macAddress);
		MsgMoteAddresses(uint16_t tosAddress, std::string mac);
		MsgMoteAddresses(uint8_t*& buffer, uint32_t& buflen);
		~MsgMoteAddresses();
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint16_t getTosAddress();
		uint64_t getMacAddress();
	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);

		uint16_t tosAddress;
		uint64_t macAddress;
};

}}}

#endif /*MSGMOTEADDRESSES_H_*/
