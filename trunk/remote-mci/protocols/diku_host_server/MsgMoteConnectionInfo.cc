#include "MsgMoteConnectionInfo.h"

namespace remote { namespace protocols { namespace diku_host_server {

MsgMoteConnectionInfo::MsgMoteConnectionInfo() : path()
{}

MsgMoteConnectionInfo::MsgMoteConnectionInfo(std::string p_mac, std::string p_path)
                     : macAddress(strtoll(p_mac.c_str(), NULL, 16)), path(p_path)
{
}

MsgMoteConnectionInfo::MsgMoteConnectionInfo(const MsgMoteConnectionInfo& o) : path()
{
	path = o.path;
	macAddress = o.macAddress;
}

MsgMoteConnectionInfo::~MsgMoteConnectionInfo()
{}

void MsgMoteConnectionInfo::operator = (const MsgMoteConnectionInfo& o)
{
	path = o.path;
	macAddress = o.macAddress;
}

uint32_t MsgMoteConnectionInfo::getLength()
{
	return sizeof(macAddress) + path.getLength();
}

uint8_t* MsgMoteConnectionInfo::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = path.write(buffer,buflen);
	buffer = writevalue(macAddress,buffer,buflen);
	return buffer;
}

uint8_t* MsgMoteConnectionInfo::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = path.read(buffer,buflen);
	buffer = readvalue(macAddress,buffer,buflen);
	return buffer;
}

void MsgMoteConnectionInfo::print(FILE* s)
{
}

MsgPayload& MsgMoteConnectionInfo::getPath()
{
	return path;
}

}}}
