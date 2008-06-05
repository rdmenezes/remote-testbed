#include "protocols/motecontrol/MsgRequest.h"

namespace remote { namespace protocols { namespace motecontrol {

MsgRequest::MsgRequest(uint8_t command) : command(command), flashImage()
{

}

MsgRequest::MsgRequest(uint8_t*& buffer, uint32_t& buflen) : flashImage()
{
	buffer=this->read(buffer,buflen);
}

MsgRequest::~MsgRequest()
{}

void MsgRequest::operator = (const MsgRequest& o)
{
	command = o.command;
	flashImage = o.flashImage;
}

uint32_t MsgRequest::getLength()
{
	uint32_t length = sizeof(command);


	if (command == MOTECOMMAND_PROGRAM)
	{
		length += flashImage.getLength();
	}
	return length;
}

uint8_t* MsgRequest::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(command,buffer,buflen);
	if (command == MOTECOMMAND_PROGRAM)
	{
		buffer = flashImage.write(buffer,buflen);
	}
	return buffer;
}

void MsgRequest::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgRequest\n");
	fprintf(s,"command: ");
	switch(command)
	{
		case MOTECOMMAND_PROGRAM:
			fprintf(s,"MOTECOMMAND_PROGRAM\n");
			fprintf(s,"flashImage: ");
			flashImage.print(s);
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
		case MOTECOMMAND_CANCELPROGRAMMING:
			fprintf(s,"MOTECOMMAND_CANCELPROGRAMMING\n");
			break;
		default:
			fprintf(s,"Invalid command!\n");
			break;
	}
}

uint8_t* MsgRequest::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(command,buffer,buflen);
	if (command == MOTECOMMAND_PROGRAM)
	{
		buffer = flashImage.read(buffer,buflen);
	}
	return buffer;
}

uint8_t MsgRequest::getCommand()
{
	return command;
}

MsgPayload& MsgRequest::getFlashImage()
{
	if ( command != MOTECOMMAND_PROGRAM )
		__THROW__ ("Cannot get flash image when command is not MOTE_PROGRAM!");
	return flashImage;
}

}}}
