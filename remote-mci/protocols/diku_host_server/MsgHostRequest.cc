#include "MsgHostRequest.h"

namespace remote { namespace protocols { namespace diku_host_server {
	
MsgHostRequest::MsgHostRequest(MsgMoteAddresses& addresses, MsgPayload& message)
            : addresses(&addresses),
              message(&message),
              deleteMsg(false)
{}

MsgHostRequest::MsgHostRequest(uint8_t*& buffer, uint32_t& buflen)
{
	buffer=this->read(buffer,buflen);
	deleteMsg = true;
}

MsgHostRequest::~MsgHostRequest()
{
	if (deleteMsg)
	{
		delete this->addresses;
		delete this->message;
	}
}
				
uint32_t MsgHostRequest::getLength()
{
	return addresses->getLength() + message->getLength();
}

uint8_t* MsgHostRequest::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = addresses->write(buffer,buflen);
	buffer = message->write(buffer,buflen);
	return buffer;
}

uint8_t* MsgHostRequest::read(uint8_t* buffer, uint32_t& buflen)
{
	addresses = new MsgMoteAddresses(buffer,buflen);
	message = new MsgPayload(buffer,buflen);
	return buffer;
}

void MsgHostRequest::print(FILE* s)
{
}
		
MsgMoteAddresses& MsgHostRequest::getMoteAddresses()
{
	return *addresses;
}

MsgPayload& MsgHostRequest::getMessage()
{
	return *message;
}

}}}
