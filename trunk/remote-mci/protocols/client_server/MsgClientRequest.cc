#include "MsgClientRequest.h"
#include "macros.h"
namespace remote { namespace protocols { namespace client_server {

MsgClientRequest::MsgClientRequest(uint8_t command)
                 : command(command), msgMoteIdList(), moteMsg()
{
}

MsgClientRequest::MsgClientRequest(uint8_t*& buffer, uint32_t& buflen) : msgMoteIdList(), moteMsg()
{
	buffer = this->read(buffer,buflen);
}

uint32_t MsgClientRequest::getLength()
{
	uint32_t length = sizeof(command);
	length += msgMoteIdList.getLength();
	switch (command)
	{
		case MSGCLIENTCOMMAND_MOTEMESSAGE:
			length += moteMsg.getLength();
			break;
		case MSGCLIENTCOMMAND_GETMOTECONTROL:
		case MSGCLIENTCOMMAND_DROPMOTECONTROL:
			break;
		default:
			__THROW__ ("Invalid command");
			break;
	}
	return length;
}

uint8_t* MsgClientRequest::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(command,buffer,buflen);
	buffer = msgMoteIdList.write(buffer,buflen);
	switch (command)
	{
		case MSGCLIENTCOMMAND_MOTEMESSAGE:
			buffer = moteMsg.write(buffer,buflen);
			break;
		case MSGCLIENTCOMMAND_GETMOTECONTROL:
		case MSGCLIENTCOMMAND_DROPMOTECONTROL:
			break;
		default:
			__THROW__ ("Invalid command");
			break;
	}
	return buffer;
}

uint8_t* MsgClientRequest::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(command,buffer,buflen);
	buffer = msgMoteIdList.read(buffer,buflen);
	switch (command)
	{
		case MSGCLIENTCOMMAND_MOTEMESSAGE:
			buffer = moteMsg.read(buffer,buflen);
			break;
		case MSGCLIENTCOMMAND_GETMOTECONTROL:
			break;
		case MSGCLIENTCOMMAND_DROPMOTECONTROL:
			break;
		default:
			__THROW__ ("Invalid command");
			break;
	}
	return buffer;
}

void MsgClientRequest::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgServerRequest\n");
	fprintf(s,"command: ");
	switch (command)
	{
		case MSGCLIENTCOMMAND_MOTEMESSAGE:
			fprintf(s,"MSGSERVERCOMMAND_MOTEMESSAGE\n");
			moteMsg.print(s);
			break;
		case MSGCLIENTCOMMAND_GETMOTECONTROL:
			fprintf(s,"MSGSERVERCOMMAND_GETMOTECONTROL\n");
			break;
		case MSGCLIENTCOMMAND_DROPMOTECONTROL:
			fprintf(s,"MSGSERVERCOMMAND_DROPMOTECONTROL\n");
			break;
		default:
			__THROW__ ("Invalid command");
			break;
	}
	msgMoteIdList.print(s);
}

uint8_t MsgClientRequest::getCommand()
{
	return command;
}

MsgMoteIdList& MsgClientRequest::getMoteIdList()
{
	if ( command != MSGCLIENTCOMMAND_GETMOTECONTROL
	     && command != MSGCLIENTCOMMAND_DROPMOTECONTROL
	     && command !=  MSGCLIENTCOMMAND_MOTEMESSAGE)
		__THROW__ ("Cannot getMoteIdList when command is not set correctly.");
	return msgMoteIdList;
}

MsgPayload& MsgClientRequest::getMoteMsg()
{
	if (command != MSGCLIENTCOMMAND_MOTEMESSAGE)
		__THROW__ ("Cannot get motemsg when type is not MSGSERVERCOMMAND_MOTEMESSAGE!");
	return moteMsg;
}

}}}
