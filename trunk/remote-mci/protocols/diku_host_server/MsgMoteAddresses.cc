#include "MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace diku_host_server {

MsgMoteAddresses::MsgMoteAddresses(uint16_t tosAddress, uint64_t macAddress)
                : tosAddress(tosAddress),
                  macAddress(macAddress)
{
}

MsgMoteAddresses::MsgMoteAddresses(uint8_t*& buffer, uint32_t& buflen)
{
	buffer = this->read(buffer,buflen);
}

MsgMoteAddresses::~MsgMoteAddresses()
{
}

uint32_t MsgMoteAddresses::getLength()
{
	return sizeof(tosAddress) + sizeof(macAddress);
}

uint8_t* MsgMoteAddresses::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(tosAddress,buffer,buflen);
	buffer = writevalue(macAddress,buffer,buflen);
	return buffer;
}

uint8_t* MsgMoteAddresses::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(tosAddress,buffer,buflen);
	buffer = readvalue(macAddress,buffer,buflen);
	return buffer;
}

void MsgMoteAddresses::print(FILE* s)
{
}

uint16_t MsgMoteAddresses::getTosAddress()
{
	return tosAddress;
}

uint64_t MsgMoteAddresses::getMacAddress()
{
	return macAddress;
}

}}}
