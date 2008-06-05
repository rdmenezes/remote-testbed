#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_MSGMOTEIDLIST_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_MSGMOTEIDLIST_H

#include <stdlib.h>
#include <list>
#include "types.h"
#include "tcputil.h"

namespace remote { namespace protocols { namespace client_server {

typedef std::list<dbkey_t> idlist_t;

class MsgMoteIdList
{
	public:
		MsgMoteIdList();
		void operator = (const MsgMoteIdList& o);
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);
		void clear();
		void addMoteId(dbkey_t p_moteId);
		bool getNextMoteId(dbkey_t &p_moteId);
	protected:
		idlist_t moteIdList;

};

}}}

#endif
