#ifndef MMSEXCEPTION_H_
#define MMSEXCEPTION_H_

#include <exception>
#include <stdio.h>
#include <string>

namespace remote { namespace protocols { 

using namespace std;
	
class MMSException : public exception
{
public:
	MMSException(const char* msg,const char* file,int line);
	~MMSException() throw ();
	const char * what () const throw ();
private:
	static long number;
	string description;
};

}}

#endif /*MMSEXCEPTION_H_*/