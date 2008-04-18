#include "MsgMoteConnectionInfo.h"

namespace remote { namespace protocols { namespace host_server {

MsgMoteConnectionInfo::MsgMoteConnectionInfo() : path()
{}

MsgMoteConnectionInfo::MsgMoteConnectionInfo(std::string p_mac, std::string p_path,
					     std::string p_platform)
	: mac(p_mac), path(p_path), platform(p_platform)
{
}

MsgMoteConnectionInfo::~MsgMoteConnectionInfo()
{}

void MsgMoteConnectionInfo::operator = (const MsgMoteConnectionInfo& o)
{
	platform = o.platform;
	path = o.path;
	mac = o.mac;
}

uint32_t MsgMoteConnectionInfo::getLength()
{
	return mac.getLength() + path.getLength() + platform.getLength();
}

uint8_t* MsgMoteConnectionInfo::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = platform.write(buffer, buflen);
	buffer = path.write(buffer, buflen);
	buffer = mac.write(buffer, buflen);
	return buffer;
}

uint8_t* MsgMoteConnectionInfo::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = platform.read(buffer, buflen);
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

std::string MsgMoteConnectionInfo::getPlatform()
{
	return platform.getString();
}

}}}
