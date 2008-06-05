#ifndef REMOTE_PROTOCOLS_MESSAGE_H
#define REMOTE_PROTOCOLS_MESSAGE_H

#include "remote.h"

#include "protocols/tcputil.h"
#include "protocols/BaseMsg.h"

namespace remote { namespace protocols {

class Message
{
	public:
		Message();
		~Message();
		bool nonBlockingRecv(int fd);
//		void recvMsg(int fd,BaseMsg& msg);
		void sendMsg(int fd,BaseMsg& msg);
		uint32_t getLength();
		uint8_t* getData();
	protected:
		uint32_t recvLen;
		uint32_t datalen;
		uint8_t* data;
};

}}

#endif
