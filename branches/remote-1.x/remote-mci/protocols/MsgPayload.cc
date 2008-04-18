#include "MsgPayload.h"
#include "macros.h"
namespace remote { namespace protocols {

MsgPayload::MsgPayload(uint8_t*& buffer, uint32_t& buflen)
{
	data = NULL;
	buffer = this->read(buffer,buflen);
}

MsgPayload::MsgPayload()
{
	data = NULL;
	length = 0;
}

MsgPayload::MsgPayload(const MsgPayload& o)
{
	data = NULL;
	setPayload(o.length,o.data);
}

MsgPayload::MsgPayload(const std::string& str)
{
	data = NULL;
	setPayload(str.length(),(uint8_t*)(str.c_str()));
}

MsgPayload::MsgPayload(BaseMsg& baseMsg)
{
	length = baseMsg.getLength();
	data = new uint8_t[length];
	uint32_t len = length;
	baseMsg.write(data,len);
}

MsgPayload::~MsgPayload()
{
	if (data)
	{
		delete data;
		data = 0;
	}
}

const MsgPayload& MsgPayload::operator = (const MsgPayload& o)
{
	setPayload(o.length,o.data);
	return *this;
}

const MsgPayload& MsgPayload::operator = (const std::string& s)
{
	setPayload(s.length(),(uint8_t*)(s.c_str()));
	return *this;
}

uint32_t MsgPayload::getLength()
{
	return sizeof(length)+length;
}

uint8_t* MsgPayload::read(uint8_t* buffer, uint32_t& buflen)
{
	buffer = readvalue(length,buffer,buflen);
	if (length > buflen)
	{
		log("Payload length %u greater than buffer length %u!\n",length,buflen);
		__THROW__ ("Payload length greater than buffer length!");
	}

	if (data)
	{
		delete data;
		data = 0;
	}

	if ( length > 0 )
	{
		data = new uint8_t[length];

		memcpy(data,buffer,length);
		buffer+=length;
		buflen-=length;
	}

	return buffer;
}

uint8_t* MsgPayload::write(uint8_t* buffer, uint32_t& buflen)
{
	buffer = writevalue(length,buffer,buflen);

	if (length > buflen)
	{
		__THROW__ ("Payload length longer than buffer!");
	}

	if ( length > 0 )
	{
		memcpy(buffer,data,length);
		buffer+=length;
		buflen-=length;
	}
	return buffer;
}

void MsgPayload::print(FILE* s)
{
	fprintf(s,"MESSAGE MsgPayload\n");
	fprintf(s,"length: %u\n",length);
}

void MsgPayload::setPayload(uint32_t p_length, const uint8_t* p_data)
{
	if (data)
	{
		delete data;
	}
	length = p_length;
	data = new uint8_t[length];
	memcpy(data,p_data,length);
}

std::string MsgPayload::getString()
{
	if (data)
	{
		return std::string((char*)data,length);
	}
	else
	{
		return std::string("");
	}
}

uint32_t MsgPayload::getDataLength()
{
	return length;
}

const uint8_t* MsgPayload::getData()
{
	return data;
}

}}
