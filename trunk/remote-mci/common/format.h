#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>

namespace remote {

static inline char *
getFormatStr(const char *format, ...)
{
	static char buf[1024];
	va_list args;

	va_start(args, format);
	if (vsnprintf(buf, sizeof(buf), format, args) >= (int) sizeof(buf))
		buf[sizeof(buf) - 1] = 0;
	va_end(args);

	return buf;
}

static inline char *
getTosStr(uint16_t tos)
{
	return getFormatStr("%u", tos);
}

}

#endif
