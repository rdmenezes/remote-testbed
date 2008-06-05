#ifndef REMOTE_PROTOCOLS_MMSEXCEPTION_H
#define REMOTE_PROTOCOLS_MMSEXCEPTION_H

#include <exception>
#include <stdio.h>
#include <string>

namespace remote { namespace protocols {

#define __THROW__(s) throw remote::protocols::MMSException(s,__FILE__,__LINE__);

class MMSException : public std::exception
{
public:
	MMSException(const char *msg, const char *file, int line);
	~MMSException() throw ();
	const char *what () const throw ();
private:
	static long number;
	std::string description;
};

}}

#endif
