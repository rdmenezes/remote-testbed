#include "protocols/client_server/MsgSession.h"

namespace remote { namespace protocols { namespace client_server {

MsgSession::MsgSession(dbkey_t sessionId) : sessionId(sessionId)
{
}

MsgSession::MsgSession(uint8_t* buffer, uint32_t& buflen)
{
	this->read(buffer,buflen);
}

MsgSession::~MsgSession()
{
}


uint32_t MsgSession::getLength()
{
	return sizeof(sessionId);
}

uint8_t* MsgSession::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(sessionId,buffer,buflen);
	return buffer;
}

uint8_t* MsgSession::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(sessionId,buffer,buflen);
	return buffer;
}

void MsgSession::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgSession\n");
	fprintf(s,"Session ID: %u\n",sessionId);
}

dbkey_t MsgSession::getSessionId()
{
	return sessionId;
}

}}}
