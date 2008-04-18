#ifndef MSGMOTECONNECTIONINFOLIST_H_
#define MSGMOTECONNECTIONINFOLIST_H_

#include <list>
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgMoteConnectionInfo.h"

namespace remote { namespace protocols { namespace host_server {

typedef std::list<MsgMoteConnectionInfo*> infolist_t;

class MsgMoteConnectionInfoList : public BaseMsg
{
	public:
		MsgMoteConnectionInfoList();
		~MsgMoteConnectionInfoList();
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		void clear();
		void addMoteInfo(MsgMoteConnectionInfo p_info);
		bool getNextMoteInfo(MsgMoteConnectionInfo& p_info);
	private:
		infolist_t infoList;
};

}}}

#endif /*MSGMOTECONNECTIONINFOLIST_H_*/
