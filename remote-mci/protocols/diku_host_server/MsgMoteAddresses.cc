#include "MsgMoteAddresses.h"
#include "common/format.h"

namespace remote { namespace protocols { namespace diku_host_server {

MsgMoteAddresses::MsgMoteAddresses(std::string p_mac, uint16_t tosAddress)
                : tosAddress(tosAddress), mac(p_mac)
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
	return sizeof(tosAddress) + mac.getLength();
}

uint8_t* MsgMoteAddresses::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(tosAddress, buffer, buflen);
	buffer = mac.write(buffer, buflen);
	return buffer;
}

uint8_t* MsgMoteAddresses::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(tosAddress, buffer, buflen);
	buffer = mac.read(buffer, buflen);
	return buffer;
}

void MsgMoteAddresses::print(FILE* s)
{
}

std::string MsgMoteAddresses::getTosAddress()
{
	return getTosStr(tosAddress);
}

std::string MsgMoteAddresses::getMac()
{
	return mac.getString();
}

}}}
