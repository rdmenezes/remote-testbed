#ifndef MSGREQUEST_H_
#define MSGREQUEST_H_
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgPayload.h"

namespace remote { namespace protocols { namespace motecontrol {
	
enum MOTECOMMAND
{
	MOTECOMMAND_PROGRAM,
	MOTECOMMAND_STATUS,
	MOTECOMMAND_RESET,
	MOTECOMMAND_STOP,
	MOTECOMMAND_START,
	MOTECOMMAND_CANCELPROGRAMMING
};

class MsgRequest : public BaseMsg
{
	public:
		MsgRequest(uint8_t command);
		MsgRequest(uint8_t*& buffer, uint32_t& buflen);
		
		~MsgRequest();
		void operator = (const MsgRequest& o);
		
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);
		
		uint8_t getCommand();
		MsgPayload& getFlashImage();		
	protected:		
		uint8_t command;
		MsgPayload flashImage;		
};

}}}
#endif /*MSGREQUEST_H_*/
