#include "MsgClientConfirm.h"

namespace remote { namespace protocols { namespace client_server {

MsgClientConfirm::MsgClientConfirm(uint8_t command, result_t result, dbkey_t mote_id)
                : command(command), result(result), mote_id(mote_id)
{
}

MsgClientConfirm::MsgClientConfirm(uint8_t command, result_t result, dbkey_t mote_id, MsgPayload& moteMsg)
                : command(command), result(result), mote_id(mote_id)
{
	this->moteMsg = moteMsg;
}

MsgClientConfirm::MsgClientConfirm(uint8_t* buffer, uint32_t& buflen)
{
	this->read(buffer,buflen);
}

uint32_t MsgClientConfirm::getLength()
{
	uint32_t length = sizeof(command)+sizeof(result)+sizeof(mote_id);
	if (command == MSGCLIENTCOMMAND_MOTEMESSAGE && result == SUCCESS)
	{
			length += moteMsg.getLength();
	}
	return length;
}

uint8_t* MsgClientConfirm::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(command,buffer,buflen);
	buffer = writevalue(result,buffer,buflen);
	buffer = writevalue(mote_id,buffer,buflen);
	if (command == MSGCLIENTCOMMAND_MOTEMESSAGE && result == SUCCESS)
	{
		buffer = moteMsg.write(buffer,buflen);
	}
	return buffer;
}

uint8_t* MsgClientConfirm::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(command,buffer,buflen);
	buffer = readvalue(result,buffer,buflen);
	buffer = readvalue(mote_id,buffer,buflen);
	if (command == MSGCLIENTCOMMAND_MOTEMESSAGE && result == SUCCESS)
	{
		buffer = moteMsg.read(buffer,buflen);
	}
	return buffer;
}

void MsgClientConfirm::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgServerConfirm\n");
	fprintf(s,"command: ");
	switch(command)
	{
		case MSGCLIENTCOMMAND_MOTEMESSAGE:
			fprintf(s,"MSGSERVERCOMMAND_MOTEMESSAGE\n");
			break;
		case MSGCLIENTCOMMAND_GETMOTECONTROL:
			fprintf(s,"MSGSERVERCOMMAND_GETMOTECONTROL\n");
			break;
		case MSGCLIENTCOMMAND_DROPMOTECONTROL:
			fprintf(s,"MSGSERVERCOMMAND_DROPMOTECONTROL\n");
			break;
		default:
			fprintf(s,"Invalid command\n");
			break;
	}

	fprintf(s,"result: ");
	switch(result)
	{
		case NOT_SUPPORTED:
			fprintf(s,"NOT_SUPPORTED\n");
			break;
		case MOTE_NOT_FOUND:
			fprintf(s,"MOTE_NOT_FOUND\n");
			break;
		case MOTE_NOT_CONTROLLED:
			fprintf(s,"MOTE_NOT_CONTROLLED\n");
			break;
		case SUCCESS:
			fprintf(s,"SUCCESS\n");
			break;
		case FAILURE:
			fprintf(s,"FAILURE\n");
			break;
		default:
			fprintf(s,"Invalid result\n");
			break;
	}

	fprintf(s,"mote_id: %u\n",mote_id);
	if (command == MSGCLIENTCOMMAND_MOTEMESSAGE && result == SUCCESS)
	{
		moteMsg.print(s);
	}
}

uint8_t MsgClientConfirm::getCommand()
{
		return command;
}

result_t MsgClientConfirm::getResult()
{
	return result;
}

dbkey_t MsgClientConfirm::getMoteId()
{
	return mote_id;
}

MsgPayload& MsgClientConfirm::getMoteMsg()
{
	if ( command != MSGCLIENTCOMMAND_MOTEMESSAGE || result != SUCCESS )
	{
		__THROW__ ("Cannot get moteMsg when command is not MSGSERVERCOMMAND_MOTEMESSAGE!");
	}
	if ( result != SUCCESS )
	{
		__THROW__ ("Cannot get moteMsg when	request is unsuccesful!");
	}
	return moteMsg;
}

}}}
