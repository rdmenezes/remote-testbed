#ifndef REMOTE_PROTOCOLS_MOTECONTROL_MSGCONFIRM_H
#define REMOTE_PROTOCOLS_MOTECONTROL_MSGCONFIRM_H

#include "BaseMsg.h"
#include "MsgRequest.h"
#include "types.h"
#include "localconstants.h"

namespace remote { namespace protocols { namespace motecontrol {

class MsgConfirm : public BaseMsg
{
	public:
	 	MsgConfirm(uint8_t command, result_t result, status_t status );
	 	MsgConfirm(uint8_t*& buffer, uint32_t& buflen);
	 	void operator = (const MsgConfirm& o);

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint8_t getCommand();
		result_t getResult();
		status_t getStatus();

	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint8_t command;
		result_t result;
		status_t status;
};
}}}
#endif
