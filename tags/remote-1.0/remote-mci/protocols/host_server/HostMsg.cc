#include "HostMsg.h"
#include "macros.h"
namespace remote { namespace protocols { namespace host_server {

HostMsg::HostMsg(MsgPlugEvent& message)
        : protocolVersion(HOST_SERVER_PROTOCOL_VERSION),
          type(HOSTMSGTYPE_PLUGEVENT),
          message(&message),
          deleteMsg(false)
{
}

HostMsg::HostMsg(MsgHostRequest& message)
        : protocolVersion(HOST_SERVER_PROTOCOL_VERSION),
          type(HOSTMSGTYPE_HOSTREQUEST),
          message(&message),
          deleteMsg(false)
{
}

HostMsg::HostMsg(MsgHostConfirm& message)
        : protocolVersion(HOST_SERVER_PROTOCOL_VERSION),
          type(HOSTMSGTYPE_HOSTCONFIRM),
          message(&message),
          deleteMsg(false)
{
}

HostMsg::HostMsg(uint8_t*& buffer, uint32_t& buflen)
{
	buffer = this->read(buffer,buflen);
	deleteMsg = true;
}

HostMsg::~HostMsg()
{
	if (deleteMsg) { delete this->message; }
}

uint32_t HostMsg::getLength()
{
	return sizeof(type) + sizeof(protocolVersion) + message->getLength();
}

uint8_t* HostMsg::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(protocolVersion,buffer,buflen);
	buffer = writevalue(type,buffer,buflen);
	buffer = message->write(buffer,buflen);
	return buffer;
}

void HostMsg::print(FILE* s)
{
	fprintf(s,"MESSAGE HostMsg\n");
	fprintf(s,"protocolVersion: %u\n",protocolVersion);
	fprintf(s,"type: ");
	switch (type)
	{
		case HOSTMSGTYPE_PLUGEVENT:
			fprintf(s,"HOSTMSGTYPE_PLUGEVENT\n");
			break;
		case HOSTMSGTYPE_HOSTREQUEST:
			fprintf(s,"HOSTMSGTYPE_HOSTMOTE\n");
			break;
		default:
			fprintf(s,"Invalid type\n");
			break;
	}
	message->print(s);
}

uint8_t* HostMsg::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(protocolVersion,buffer,buflen);
	if (protocolVersion != HOST_SERVER_PROTOCOL_VERSION)
	{
		__THROW__("Host/Server protocol version conflict!");
	}
	buffer = readvalue(type,buffer,buflen);
	switch (type)
	{
		case HOSTMSGTYPE_PLUGEVENT:
			message = new MsgPlugEvent(buffer,buflen);
			break;
		case HOSTMSGTYPE_HOSTREQUEST:
			message = new MsgHostRequest(buffer,buflen);
			break;
		case HOSTMSGTYPE_HOSTCONFIRM:
			message = new MsgHostConfirm(buffer,buflen);
			break;
		default:
			__THROW__("Unknown type of HostMsg!");
		break;
	}
	return buffer;
}

uint32_t HostMsg::getProtocolVersion()
{
	return protocolVersion;
}

uint8_t HostMsg::getType()
{
	return type;
}

MsgHostRequest& HostMsg::getHostRequest()
{
	if (type != HOSTMSGTYPE_HOSTREQUEST)
		__THROW__ ("Cannot get MsgHostRequest when type is not HOSTMSGTYPE_HOSTREQUEST!");
	return *((MsgHostRequest*)message);
}

MsgHostConfirm& HostMsg::getHostConfirm()
{
	if (type != HOSTMSGTYPE_HOSTCONFIRM)
		__THROW__ ("Cannot get MsgHostConfirm when type is not HOSTMSGTYPE_HOSTCONFIRM!");
	return *((MsgHostConfirm*)message);
}

MsgPlugEvent& HostMsg::getPlugEvent()
{
	if (type != HOSTMSGTYPE_PLUGEVENT)
		__THROW__ ("Cannot get plugevent when type is not HOST_PLUGEVENT!");
	return *((MsgPlugEvent*)message);
}

}}}
