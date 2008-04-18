#include "MsgPlugEvent.h"

namespace remote { namespace protocols { namespace host_server {

MsgPlugEvent::MsgPlugEvent(uint8_t type) : type(type), infoList()
{
}

MsgPlugEvent::MsgPlugEvent(uint8_t*& buffer, uint32_t& buflen) : infoList()
{
	buffer = this->read(buffer,buflen);
}

MsgPlugEvent::~MsgPlugEvent()
{}

uint32_t MsgPlugEvent::getLength()
{
	uint32_t length = sizeof(type);
	length += infoList.getLength();
	return length;
}

uint8_t* MsgPlugEvent::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(type,buffer,buflen);
	buffer = infoList.write(buffer,buflen);
	return buffer;
}

uint8_t* MsgPlugEvent::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(type,buffer,buflen);
	buffer = infoList.read(buffer,buflen);
	return buffer;
}

void MsgPlugEvent::print(FILE* s)
{
}

uint8_t MsgPlugEvent::getType()
{
	return type;
}

MsgMoteConnectionInfoList& MsgPlugEvent::getInfoList()
{
	return infoList;
}

}}}
