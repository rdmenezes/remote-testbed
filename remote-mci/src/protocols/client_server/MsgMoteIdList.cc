#include "protocols/client_server/MsgMoteIdList.h"

namespace remote { namespace protocols { namespace client_server {

MsgMoteIdList::MsgMoteIdList() : moteIdList()
{
}

void MsgMoteIdList::operator = (const MsgMoteIdList& o)
{
	moteIdList = o.moteIdList;
}

uint32_t MsgMoteIdList::getLength()
{
	// length and elements
	return sizeof(uint16_t)+sizeof(uint32_t)*moteIdList.size();
}

uint8_t* MsgMoteIdList::write(uint8_t* buffer, uint32_t& buflen)
{
	uint16_t count;
	dbkey_t moteId;

	count = moteIdList.size();

	buffer = writevalue(count,buffer,buflen);

	idlist_t::iterator i;
	for ( i = moteIdList.begin(); i != moteIdList.end(); i++)
	{
		moteId = *i;
		buffer = writevalue(moteId,buffer,buflen);
	}
	return buffer;
}

uint8_t* MsgMoteIdList::read(uint8_t* buffer, uint32_t& buflen)
{
	uint16_t count,i;
	dbkey_t moteId;
	moteIdList.clear();

	buffer = readvalue(count,buffer,buflen);

	for (i=0; i < count; i++)
	{
		buffer = readvalue(moteId,buffer,buflen);
		moteIdList.push_back(moteId);
	}
	return buffer;
}

void MsgMoteIdList::print(FILE* s)
{
	uint16_t count;
	dbkey_t moteId;
	count = moteIdList.size();
	fprintf(s,"MESSAGE MsgMoteIdList\n");
	fprintf(s,"%u elements:\n",count);

	idlist_t::iterator i;
	for ( i = moteIdList.begin(); i != moteIdList.end(); i++)
	{
		moteId = *i;
		fprintf(s,"%u\n",moteId);
	}
}

void MsgMoteIdList::clear()
{
		moteIdList.clear();
}

void MsgMoteIdList::addMoteId(dbkey_t p_moteId)
{
	moteIdList.push_back(p_moteId);
}

bool MsgMoteIdList::getNextMoteId(dbkey_t &p_moteId)
{
	if (moteIdList.empty()) return false;
	else
	{
		p_moteId = moteIdList.front();
		moteIdList.pop_front();
		return true;
	}
}

}}}
