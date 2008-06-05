#ifndef REMOTE_PROTOCOLS_HOST_SERVER_MSGPLUGEVENT_H
#define REMOTE_PROTOCOLS_HOST_SERVER_MSGPLUGEVENT_H

#include "protocols/BaseMsg.h"
#include "protocols/host_server/MsgMoteConnectionInfoList.h"

namespace remote { namespace protocols { namespace host_server {

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

#endif
