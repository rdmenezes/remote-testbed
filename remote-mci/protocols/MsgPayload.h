#ifndef MSGPAYLOAD_H_
#define MSGPAYLOAD_H_
#include "tcputil.h"
#include "BaseMsg.h"

namespace remote { namespace protocols { 
	
class MsgPayload : public BaseMsg
{
	public:
		MsgPayload();
		MsgPayload(uint8_t*& buffer, uint32_t& buflen);
		MsgPayload(const MsgPayload& o);
		MsgPayload(const std::string& str);
		MsgPayload(BaseMsg& baseMsg);
		~MsgPayload();
		
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);
		
		const MsgPayload& operator = (const MsgPayload& o);
		const MsgPayload& operator = (const std::string& s);
		void setPayload(uint32_t p_length, const uint8_t* p_data);
		std::string getString();
		uint32_t getDataLength();
		const uint8_t* getData();
	protected:
		uint32_t length;
		uint8_t* data;	
};

}}

#endif /*MSGPAYLOAD_H_*/
