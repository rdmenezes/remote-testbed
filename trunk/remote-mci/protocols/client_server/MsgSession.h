#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_MSGSESSION_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_MSGSESSION_H
#include "BaseMsg.h"
#include "types.h"

namespace remote { namespace protocols { namespace client_server {

class MsgSession : public BaseMsg
{
	public:
		MsgSession(dbkey_t sessionId);
		MsgSession(uint8_t* buffer, uint32_t& buflen);
		~MsgSession();

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		dbkey_t getSessionId();
	protected:
		dbkey_t sessionId;
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
};

}}}

#endif
