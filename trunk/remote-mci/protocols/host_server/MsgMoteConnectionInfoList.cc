#include "MsgMoteConnectionInfoList.h"

namespace remote { namespace protocols { namespace host_server {

MsgMoteConnectionInfoList::MsgMoteConnectionInfoList()
{
}

MsgMoteConnectionInfoList::~MsgMoteConnectionInfoList()
{
}

uint32_t MsgMoteConnectionInfoList::getLength()
{
	MsgMoteConnectionInfo* moteInfo;
	infolist_t::iterator i;
	uint32_t length = sizeof(uint16_t);

	for ( i = infoList.begin(); i != infoList.end(); i++)
	{
		moteInfo = *i;
		length += moteInfo->getLength();
	}
	return length;
}

uint8_t* MsgMoteConnectionInfoList::write(uint8_t* buffer, uint32_t& buflen)
{
	uint16_t count;
	MsgMoteConnectionInfo* moteInfo;

	count = infoList.size();

	if (count>1000)
	{
		__THROW__ ("Illegal infoList count > 1000!");
	}

	buffer = writevalue(count,buffer,buflen);

	infolist_t::iterator i;
	for ( i = infoList.begin(); i != infoList.end(); i++)
	{
		moteInfo = *i;
		buffer = moteInfo->write(buffer,buflen);
	}
	return buffer;
}

uint8_t* MsgMoteConnectionInfoList::read(uint8_t* buffer, uint32_t& buflen)
{
	uint16_t count,i;
	MsgMoteConnectionInfo* moteInfo;

	infoList.clear();

	buffer = readvalue(count,buffer,buflen);

	if (count>1000)
	{
		__THROW__ ("Illegal infoList count > 1000!");
	}

	for (i=0; i < count; i++)
	{
		moteInfo = new MsgMoteConnectionInfo();
		buffer = moteInfo->read(buffer,buflen);
		infoList.push_back(moteInfo);
	}
	return buffer;
}

void MsgMoteConnectionInfoList::print(FILE* s)
{
}

void MsgMoteConnectionInfoList::clear()
{
	infolist_t::iterator i;
	for ( i = infoList.begin(); i != infoList.end(); i++)
	{
		delete *i;
	}
	infoList.clear();
}

void MsgMoteConnectionInfoList::addMoteInfo(MsgMoteConnectionInfo p_info)
{
	MsgMoteConnectionInfo* info = new MsgMoteConnectionInfo(p_info);
	infoList.push_back(info);
}

bool MsgMoteConnectionInfoList::getNextMoteInfo(MsgMoteConnectionInfo& p_info)
{
	if (infoList.empty()) return false;
	else
	{
		p_info = *(infoList.front());
		delete infoList.front();
		infoList.pop_front();
		return true;
	}
}

}}}
