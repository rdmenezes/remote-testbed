#include "protocols/host_server/MsgMoteAddresses.h"

namespace remote { namespace protocols { namespace host_server {

MsgMoteAddresses::MsgMoteAddresses(std::string p_mac, std::string netAddress)
                : netAddress(netAddress), mac(p_mac)
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
	return netAddress.getLength() + mac.getLength();
}

uint8_t* MsgMoteAddresses::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = mac.write(buffer, buflen);
	buffer = netAddress.write(buffer, buflen);
	return buffer;
}

uint8_t* MsgMoteAddresses::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = mac.read(buffer, buflen);
	buffer = netAddress.read(buffer, buflen);
	return buffer;
}

void MsgMoteAddresses::print(FILE* s)
{
}

std::string MsgMoteAddresses::getNetAddress()
{
	return netAddress.getString();
}

std::string MsgMoteAddresses::getMac()
{
	return mac.getString();
}

}}}
