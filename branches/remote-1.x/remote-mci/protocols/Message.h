#ifndef MESSAGE_H_
#define MESSAGE_H_
#include "tcputil.h"
#include "BaseMsg.h"
#include <netinet/in.h>
#include <errno.h>

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

#endif /*MESSAGE_H_*/
