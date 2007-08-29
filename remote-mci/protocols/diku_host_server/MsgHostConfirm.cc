#include "MsgHostConfirm.h"

namespace remote { namespace protocols { namespace diku_host_server {
	
MsgHostConfirm::MsgHostConfirm(uint8_t status,MsgMoteAddresses& addresses, MsgPayload& message)
            : status(status),
              addresses(&addresses),
              message(&message),
              deleteMsg(false)
{}

MsgHostConfirm::MsgHostConfirm(uint8_t*& buffer, uint32_t& buflen)
{
	buffer=this->read(buffer,buflen);
	deleteMsg = true;
}

MsgHostConfirm::~MsgHostConfirm()
{
	if (deleteMsg)
	{
		delete this->addresses;
		delete this->message;
	}
}
				
uint32_t MsgHostConfirm::getLength()
{
	return sizeof(status) + addresses->getLength() + message->getLength();
}

uint8_t* MsgHostConfirm::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(status,buffer,buflen);
	buffer = addresses->write(buffer,buflen);
	buffer = message->write(buffer,buflen);
	return buffer;
}

uint8_t* MsgHostConfirm::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(status,buffer,buflen);
	addresses = new MsgMoteAddresses(buffer,buflen);
	message = new MsgPayload(buffer,buflen);
	return buffer;
}

void MsgHostConfirm::print(FILE* s)
{
}

uint8_t MsgHostConfirm::getStatus()
{
	return status;
}
		
MsgMoteAddresses& MsgHostConfirm::getMoteAddresses()
{
	return *addresses;
}

MsgPayload& MsgHostConfirm::getMessage()
{
	return *message;
}

}}}
