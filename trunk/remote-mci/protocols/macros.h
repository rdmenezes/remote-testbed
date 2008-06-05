#ifndef MACROS_H_
#define MACROS_H_
#include <time.h>
#include <stdarg.h>
#include "MMSException.h"

namespace remote
{

inline void log(const char* s,...)
{
	va_list ap;
	va_start(ap, s);
	time_t t;
	time(&t);
	char* time = ctime(&t);
	printf("%s: ",strsep(&time,"\n"));
	vprintf(s,ap);
	va_end(ap);
	fflush(stdout);
}

}

#endif /*MACROS_H_*/
