#include "MsgMoteConnectionInfo.h"

namespace remote { namespace protocols { namespace diku_host_server {

MsgMoteConnectionInfo::MsgMoteConnectionInfo() : path()
{}

MsgMoteConnectionInfo::MsgMoteConnectionInfo(std::string p_mac, std::string p_path)
                     : mac(p_mac), path(p_path)
{
}

MsgMoteConnectionInfo::MsgMoteConnectionInfo(const MsgMoteConnectionInfo& o) : path()
{
	path = o.path;
	mac = o.mac;
}

MsgMoteConnectionInfo::~MsgMoteConnectionInfo()
{}

void MsgMoteConnectionInfo::operator = (const MsgMoteConnectionInfo& o)
{
	path = o.path;
	mac = o.mac;
}

uint32_t MsgMoteConnectionInfo::getLength()
{
	return mac.getLength() + path.getLength();
}

uint8_t* MsgMoteConnectionInfo::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = path.write(buffer, buflen);
	buffer = mac.write(buffer, buflen);
	return buffer;
}

uint8_t* MsgMoteConnectionInfo::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = path.read(buffer, buflen);
	buffer = mac.read(buffer, buflen);
	return buffer;
}

void MsgMoteConnectionInfo::print(FILE* s)
{
}

MsgPayload& MsgMoteConnectionInfo::getPath()
{
	return path;
}

std::string MsgMoteConnectionInfo::getMac()
{
	return mac.getString();
}

}}}
