#ifndef MSGMOTECONNECTIONINFO_H_
#define MSGMOTECONNECTIONINFO_H_

#include "BaseMsg.h"
#include "MsgPayload.h"

namespace remote { namespace protocols { namespace diku_host_server {

class MsgMoteConnectionInfo : public BaseMsg
{
	public:
		MsgMoteConnectionInfo();
		MsgMoteConnectionInfo(std::string p_mac, std::string p_path,
				      std::string p_platform);
		virtual ~MsgMoteConnectionInfo();
		void operator = (const MsgMoteConnectionInfo& o);

		uint32_t getLength();
		uint8_t* write(uint8_t* buffer, uint32_t& buflen);
		uint8_t* read(uint8_t* buffer, uint32_t& buflen);
		void print(FILE* s);

		void read(int fd);
		void write(int fd);
		MsgPayload& getPath();
		std::string getMac();
		std::string getPlatform();
	private:
		MsgPayload mac;
		MsgPayload path;
		MsgPayload platform;
};

}}}

#endif /*MSGMOTECONNECTIONINFO_H_*/
