#ifndef BASEMSG_H_
#define BASEMSG_H_
#include "tcputil.h"

namespace remote { namespace protocols { 
	
class BaseMsg
{
	public:
		virtual ~BaseMsg(){};
		virtual uint32_t getLength() = 0;
		virtual uint8_t* write(uint8_t* buffer, uint32_t& buflen) = 0;
		virtual void print(FILE* s) = 0;
	protected:
		virtual uint8_t* read(uint8_t* buffer, uint32_t& buflen) = 0;
};

}}

#endif /*BASEMSG_H_*/
