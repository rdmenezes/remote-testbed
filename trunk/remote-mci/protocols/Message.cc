#include "Message.h"
#include "macros.h"
namespace remote { namespace protocols { 
	
Message::Message()
{
	data = NULL;
	datalen = 0;
	recvLen = 0;
}

Message::~Message()
{
	if (data) delete data;
	datalen = 0;
}

bool Message::nonBlockingRecv(int fd)
{
	char* p_datalen = (char*)&datalen;
	int r;
	uint32_t targetRecvLen;
	
	if ( recvLen < sizeof(datalen))
	{
		// still at the datalen part
		r = read(fd,p_datalen+recvLen,sizeof(datalen)-recvLen);
		if (r<=0)
		{
			int e = errno;
			log("Recv error %i\n",e);
			__THROW__("Unable to read message length!");
		}
		recvLen += r;
	}
	if ( recvLen < sizeof(datalen) )
	{
		return false;
	}
	if ( recvLen == sizeof(datalen) )
	{
		datalen = ntohl(datalen);
		if (datalen > 512000) __THROW__("Data too long!");
		if (data) delete data;
		data = new uint8_t[datalen];
	}
	targetRecvLen = datalen + sizeof(datalen);
	if ( recvLen >= sizeof(datalen) && recvLen < targetRecvLen )
	{
		r = read(fd,data+recvLen-sizeof(datalen),targetRecvLen-recvLen);
		if (r<=0)
		{
			int e = errno;
			log("Recv error %i\n",e);
			__THROW__ ("Unable to read message data!");
		}
		recvLen += r;
//		printf("Read %u of %u bytes.\n",recvLen-sizeof(datalen),datalen);
	}
	if (recvLen == targetRecvLen)
	{
		recvLen = 0;
		return true;
	}
	return false;
}


/*void Message::recvMsg(int fd,BaseMsg& msg)
{
	if (data) delete data;
	// read length, then data
	if (!recv(fd,datalen))
		__THROW__ ("Unable to receive datalen!");
//	printf("Reading %u bytes of message data\n",datalen);
	data = new uint8_t[datalen];
	if (recv(fd, (char*)data, datalen, MSG_WAITALL) <= 0)
		__THROW__ ("Unable to send data!");

	uint32_t len = datalen;
	//	let msg read the buffer
	msg.read(data,len);
//	msg.print(stdout);
//	printf("Done reading %u bytes of message data\n\n\n",datalen);
}*/

void Message::sendMsg(int fd,BaseMsg& msg)
{
	uint32_t len;
	if (data) delete data;
	// allocate data 
	datalen = msg.getLength();
	data = new uint8_t[datalen];
	len = datalen;
//	printf("Writing %u bytes of message data\n",datalen);
	
	uint8_t* d = data;
	msg.write(d,len);
//	msg.print(stdout);
	
	
	// write length, then data
	if (!send(fd,datalen))
		__THROW__ ("Unable to send datalen!");
	if (::send(fd, (char*)data, datalen, 0) != ((int64_t)datalen))
		__THROW__ ("Unable to send data!");
//	printf("Done writing %u bytes of message data\n\n\n",datalen);
}

uint32_t Message::getLength()
{
	return datalen;
}

uint8_t* Message::getData()
{
	return data;
}
}}
