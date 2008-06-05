#ifndef REMOTE_PROTOCOLS_MOTECONTROL_MOTEMSG_H
#define REMOTE_PROTOCOLS_MOTECONTROL_MOTEMSG_H
#include "BaseMsg.h"
#include "MsgRequest.h"
#include "MsgConfirm.h"
#include "MsgPayload.h"

namespace remote { namespace protocols { namespace motecontrol {

enum MoteMsgType
{
	MOTEMSGTYPE_REQUEST,
	MOTEMSGTYPE_CONFIRM,
	MOTEMSGTYPE_DATA,
};

class MoteMsg : public BaseMsg
{
	public:
		MoteMsg(uint8_t*& buffer, uint32_t& buflen);
		MoteMsg(MsgRequest& message);
		MoteMsg(MsgConfirm& message);
		MoteMsg(MsgPayload& message);
		~MoteMsg();

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint32_t getProtocolVersion();
		uint8_t getType();
		MsgRequest& getRequest();
		MsgConfirm& getConfirm();
		MsgPayload& getData();
	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint32_t protocolVersion;
		uint8_t type;
		BaseMsg* message;
		bool deleteMsg;
};
}}}
#endif
