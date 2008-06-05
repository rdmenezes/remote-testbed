#ifndef REMOTE_PROTOCOLS_HOST_SERVER_MSGHOSTCONFIRM_H
#define REMOTE_PROTOCOLS_HOST_SERVER_MSGHOSTCONFIRM_H

#include "protocols/BaseMsg.h"
#include "protocols/MsgPayload.h"
#include "protocols/host_server/MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace host_server {

enum MSGHOSTCONFIRM_STATUS {
	MSGHOSTCONFIRM_OK,
	MSGHOSTCONFIRM_UNKNOWN_MOTE
};

class MsgHostConfirm : public BaseMsg
{
	public:
		MsgHostConfirm(uint8_t status, MsgMoteAddresses& adressess, MsgPayload& message);
		MsgHostConfirm(uint8_t*& buffer, uint32_t& buflen);
		~MsgHostConfirm();

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint8_t getStatus();
		MsgMoteAddresses& getMoteAddresses();
		MsgPayload& getMessage();

	protected:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint8_t status;
		MsgMoteAddresses* addresses;
		MsgPayload* message;
		bool deleteMsg;
};

}}}
#endif
