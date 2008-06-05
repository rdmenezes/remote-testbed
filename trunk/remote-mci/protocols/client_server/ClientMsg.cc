#include "ClientMsg.h"

namespace remote { namespace protocols { namespace client_server {

ClientMsg::ClientMsg(uint8_t*& buffer, uint32_t& buflen)
{
	buffer = this->read(buffer,buflen);
	deleteMsg = true;
}

ClientMsg::ClientMsg(MsgSession& message)
          : type(CLIENTMSG_SESSION),
            message(&message),
            deleteMsg(false)
{}

ClientMsg::ClientMsg(MsgClientRequest& message)
         : type(CLIENTMSG_CLIENTREQUEST),
            message(&message),
            deleteMsg(false)
{}

ClientMsg::ClientMsg(MsgClientConfirm& message)
         : type(CLIENTMSG_CLIENTCONFIRM),
            message(&message),
            deleteMsg(false)
{}

ClientMsg::~ClientMsg()
{
	if (deleteMsg) { delete this->message; }
}

uint32_t ClientMsg::getLength()
{
	return sizeof(protocolVersion) + sizeof(type) + message->getLength();
}

uint8_t* ClientMsg::write(uint8_t* buffer, uint32_t& buflen)
{
	protocolVersion = CurrentProtocolVersion;
	buffer = writevalue(protocolVersion,buffer,buflen);
	buffer = writevalue(type,buffer,buflen);
	buffer = message->write(buffer,buflen);
	return buffer;
}

uint8_t* ClientMsg::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(protocolVersion,buffer,buflen);
	buffer = readvalue(type,buffer,buflen);
	switch (type)
	{
		case CLIENTMSG_SESSION:
			message = new MsgSession(buffer,buflen);
			break;
		case CLIENTMSG_CLIENTREQUEST:
			message = new MsgClientRequest(buffer,buflen);
			break;
		case CLIENTMSG_CLIENTCONFIRM:
			message = new MsgClientConfirm(buffer,buflen);
			break;
		default:
			__THROW__ ("Invalid serverMsg");
			break;
	}
	return buffer;
}

void ClientMsg::print(FILE* s)
{
	fprintf(s,"MESSAGE ServerMsg\n");
	fprintf(s,"protocolVersion: %u\n",protocolVersion);
	fprintf(s,"type: ");
	switch (type)
	{
		case CLIENTMSG_CLIENTREQUEST:
			fprintf(s,"SERVERMSG_SERVERREQUEST\n");
			break;
		case CLIENTMSG_CLIENTCONFIRM:
			fprintf(s,"SERVERMSG_SERVERCONFIRM\n");
			break;
		case CLIENTMSG_SESSION:
			fprintf(s,"SERVERMSG_SESSION\n");
			break;
		default:
			__THROW__ ("Invalid serverMsg");
			break;
	}
	message->print(s);
}

uint8_t ClientMsg::getType()
{
	return type;
}

uint32_t ClientMsg::getProtocolVersion()
{
	return protocolVersion;
}

MsgClientRequest& ClientMsg::getClientRequest()
{
	if (type != CLIENTMSG_CLIENTREQUEST)
		__THROW__ ("Cannot get ServerRequest when type is not SERVERMSG_SERVERREQUEST!");
	return *((MsgClientRequest*)message);
}

MsgClientConfirm& ClientMsg::getClientConfirm()
{
	if (type != CLIENTMSG_CLIENTCONFIRM)
		__THROW__ ("Cannot get ServerConfirm when type is not SERVERMSG_SERVERCONFIRM!");
	return *((MsgClientConfirm*)message);
}

MsgSession& ClientMsg::getSession()
{
	if (type != CLIENTMSG_SESSION)
		__THROW__ ("Cannot getSession when command is not SERVERMSG_SESSION!");
	return *((MsgSession*)message);
}
}}}
