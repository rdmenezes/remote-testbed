#include "MMSException.h"

namespace remote { namespace protocols {

MMSException::MMSException(const char* msg,const char* file,int line)
{
	number++;
	char* buffer = new char[1000];
	sprintf(buffer,"MMS #%ld: %s at %s:%u",number,msg,file,line);
	description = buffer;
	delete buffer;
}

MMSException::~MMSException() throw ()
{

}

const char * MMSException::what () const throw ()
{
	return description.c_str();
}

long MMSException::number = 0;

}}
