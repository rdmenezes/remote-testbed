#include "MsgConfirm.h"
namespace remote { namespace protocols { namespace motecontrol {

MsgConfirm::MsgConfirm(uint8_t command, result_t result, status_t status )
          : command(command), result(result), status(status)
{
}

MsgConfirm::MsgConfirm(uint8_t*& buffer, uint32_t& buflen)
{
	buffer = this->read(buffer,buflen);
}

void MsgConfirm::operator = (const MsgConfirm& o)
{
	command = o.command;
	result = o.result;
	status = o.status;
}

uint32_t MsgConfirm::getLength()
{
	return sizeof(command)+sizeof(result)+sizeof(status);
}

uint8_t* MsgConfirm::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(command,buffer,buflen);
	buffer = writevalue(result,buffer,buflen);
	buffer = writevalue(status,buffer,buflen);
	return buffer;
}

uint8_t* MsgConfirm::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(command,buffer,buflen);
	buffer = readvalue(result,buffer,buflen);
	buffer = readvalue(status,buffer,buflen);
	return buffer;
}

void MsgConfirm::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgConfirm\n");
	fprintf(s,"command: ");
	switch(command)
	{
		case MOTECOMMAND_PROGRAM:
			fprintf(s,"MOTECOMMAND_PROGRAM\n");
			break;
		case MOTECOMMAND_STATUS:
			fprintf(s,"MOTECOMMAND_STATUS\n");
			break;
		case MOTECOMMAND_RESET:
			fprintf(s,"MOTECOMMAND_RESET\n");
			break;
		case MOTECOMMAND_STOP:
			fprintf(s,"MOTECOMMAND_STOP\n");
			break;
		case MOTECOMMAND_START:
			fprintf(s,"MOTECOMMAND_START\n");
			break;
		default:
			fprintf(s,"Invalid command!\n");
			break;
	}
	fprintf(s,"result: ");
	switch(result)
	{
		case NOT_SUPPORTED:
			fprintf(s,"NOT_SUPPORTED\n");
			break;
		case SUCCESS:
			fprintf(s,"SUCCESS\n");
			break;
		case FAILURE:
			fprintf(s,"FAILURE\n");
			break;
		default:
			fprintf(s,"Invalid result!\n");
			break;
	}
	fprintf(s,"status: ");
	switch(status)
	{
		case MOTE_STATUS_UNKNOWN:
			fprintf(s,"MOTE_STATUS_UNKNOWN\n");
			break;
		case MOTE_UNAVAILABLE:
			fprintf(s,"MOTE_UNAVAILABLE\n");
			break;
		case MOTE_STOPPED:
			fprintf(s,"MOTE_STOPPED\n");
			break;
		case MOTE_RUNNING:
			fprintf(s,"MOTE_RUNNING\n");
			break;
		case MOTE_PROGRAMMING:
			fprintf(s,"MOTE_PROGRAMMING\n");
			break;
		default:
			fprintf(s,"Invalid status!\n");
			break;
	}
}

uint8_t MsgConfirm::getCommand()
{
	return command;
}

result_t MsgConfirm::getResult()
{
	return result;
}

status_t MsgConfirm::getStatus()
{
	return status;
}

}}}
