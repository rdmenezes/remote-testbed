#include "MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace diku_host_server {

MsgMoteAddresses::MsgMoteAddresses(std::string p_mac, std::string tosAddress)
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
	return tosAddress.getLength() + mac.getLength();
}

uint8_t* MsgMoteAddresses::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = mac.write(buffer, buflen);
	buffer = tosAddress.write(buffer, buflen);
	return buffer;
}

uint8_t* MsgMoteAddresses::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = mac.read(buffer, buflen);
	buffer = tosAddress.read(buffer, buflen);
	return buffer;
}

void MsgMoteAddresses::print(FILE* s)
{
}

std::string MsgMoteAddresses::getTosAddress()
{
	return tosAddress.getString();
}

std::string MsgMoteAddresses::getMac()
{
	return mac.getString();
}

}}}
