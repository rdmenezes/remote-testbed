#include "MoteMsg.h"
#include "macros.h"
namespace remote { namespace protocols { namespace motecontrol {

MoteMsg::MoteMsg(uint8_t*& buffer, uint32_t& buflen)
{
	buffer = this->read(buffer,buflen);
	deleteMsg = true;
}

MoteMsg::MoteMsg(MsgRequest& message)
        : type(MOTEMSGTYPE_REQUEST),
          message(&message),
          deleteMsg(false)
{}

MoteMsg::MoteMsg(MsgConfirm& message)
        : type(MOTEMSGTYPE_CONFIRM),
          message(&message),
          deleteMsg(false)
{}

MoteMsg::MoteMsg(MsgPayload& message)
        : type(MOTEMSGTYPE_DATA),
          message(&message),
          deleteMsg(false)
{}

MoteMsg::~MoteMsg()
{
	if (deleteMsg) {delete this->message;}
}


uint32_t MoteMsg::getLength()
{
	return sizeof(protocolVersion) + sizeof(type) + message->getLength();
}

uint8_t* MoteMsg::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(protocolVersion,buffer,buflen);
	buffer = writevalue(type,buffer,buflen);
	buffer = message->write(buffer,buflen);
	return buffer;
}

uint8_t* MoteMsg::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(protocolVersion,buffer,buflen);
	// TODO: check protocolVersion
	buffer = readvalue(type,buffer,buflen);
	switch (type)
	{
		case MOTEMSGTYPE_REQUEST:
			message = new MsgRequest(buffer,buflen);
			break;
		case MOTEMSGTYPE_CONFIRM:
			message = new MsgConfirm(buffer,buflen);
			break;
		case MOTEMSGTYPE_DATA:
			message = new MsgPayload(buffer,buflen);
			break;
	}
	return buffer;
}

void MoteMsg::print(FILE* s)
{
	fprintf(s,"MESSAGE MoteMsg\n");
	fprintf(s,"protocolVersion: %u\n",protocolVersion);
	fprintf(s,"type: ");
	switch (type)
	{
		case MOTEMSGTYPE_REQUEST:
			fprintf(s,"MOTEMSGTYPE_REQUEST\n");
			break;
		case MOTEMSGTYPE_CONFIRM:
			fprintf(s,"MOTEMSGTYPE_CONFIRM\n");
			break;
		case MOTEMSGTYPE_DATA:
			fprintf(s,"MOTEMSGTYPE_DATA\n");
			break;
		default:
			fprintf(s,"Invalid type!\n");
	}
	message->print(s);
}

uint32_t MoteMsg::getProtocolVersion()
{
	return protocolVersion;
}

uint8_t MoteMsg::getType()
{
	return type;
}

MsgRequest& MoteMsg::getRequest()
{
	if (type != MOTEMSGTYPE_REQUEST)
		__THROW__ ("Cannot getRequest when type is not MOTEMSGTYPE_REQUEST!");
	return *((MsgRequest*)message);
}

MsgConfirm& MoteMsg::getConfirm()
{
	if (type != MOTEMSGTYPE_CONFIRM)
		__THROW__ ("Cannot getConfirm when type is not MOTEMSGTYPE_CONFIRM!");
	return *((MsgConfirm*)message);
}

MsgPayload& MoteMsg::getData()
{
	if (type != MOTEMSGTYPE_DATA)
		__THROW__ ("Cannot getData when type is not MOTEMSGTYPE_DATA!");
	return *((MsgPayload*)message);
}
}}}
