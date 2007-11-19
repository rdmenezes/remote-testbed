#ifndef MSGPLUGEVENT_H_
#define MSGPLUGEVENT_H_
#include "tcputil.h"
#include "BaseMsg.h"
#include "MsgMoteConnectionInfoList.h"

namespace remote { namespace protocols { namespace diku_host_server {

enum plugeventtype
{
	PLUG_MOTES = 0,
	UNPLUG_MOTES = 1
};

class MsgPlugEvent : public BaseMsg
{
	public:
		MsgPlugEvent(uint8_t type);
		MsgPlugEvent(uint8_t*& buffer, uint32_t& buflen);
		virtual ~MsgPlugEvent();
		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		uint8_t getType();
		MsgMoteConnectionInfoList& getInfoList();

	private:
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		uint8_t type;
		MsgMoteConnectionInfoList infoList;
};

}}}

#endif /*MSGPLUGEVENT_H_*/
