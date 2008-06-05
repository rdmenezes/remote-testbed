/* Put log messages either to stdout or syslog.
 *
 * Assimilated from git.git::daemon.c on 2007-05-19. */

#include "util/Log.h"

namespace remote { namespace util {

void
Log::open(const char *ident, int level, const FILE *file)
{
	if (use_syslog()) {
		closelog();
	}

	Log::ident = ident;
	Log::level = level;
	Log::file = file;

	if (use_syslog()) {
		openlog(Log::ident, 0, LOG_DAEMON);
	}
}

void
Log::put(int priority, const char *format, va_list params)
{
	/* We should do a single write so that it is atomic and output
	 * of several processes do not get intermingled. */
	char buf[1024];
	int buflen;
	int maxlen, msglen;
	time_t t;
	char *timestr;

	if (ignore(priority))
		return;

	/* sizeof(buf) should be big enough for "[pid] \n" */
	buflen = snprintf(buf, sizeof(buf), "[%ld] ", (long) getpid());

	maxlen = sizeof(buf) - buflen - 1; /* -1 for our own LF */
	msglen = vsnprintf(buf + buflen, maxlen, format, params);

	if (use_syslog()) {
		syslog(priority, "%s", buf);
		return;
	}

	/* maxlen counted our own LF but also counts space given to
	 * vsnprintf for the terminating NUL.  We want to make sure that
	 * we have space for our own LF and NUL after the "meat" of the
	 * message, so truncate it at maxlen - 1.
	 */
	if (msglen > maxlen - 1)
		msglen = maxlen - 1;
	else if (msglen < 0)
		msglen = 0; /* Protect against weird return values. */
	buflen += msglen;

	buf[buflen] = '\0';

	time(&t);
	timestr = ctime(&t);
	fprintf((FILE *) Log::file, "%s - %s %s\n", strsep(&timestr, "\n"), Log::ident, buf);
}

void
Log::fatal(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(Log::FATAL, format, params);
	va_end(params);
}

void
Log::error(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(Log::ERROR, format, params);
	va_end(params);
}

void
Log::warn(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(Log::WARN, format, params);
	va_end(params);
}

void
Log::info(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(Log::INFO, format, params);
	va_end(params);
}

void
Log::debug(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(Log::DEBUG, format, params);
	va_end(params);
}

const char *Log::ident;
const FILE *Log::file;
int Log::level;
const FILE *Log::SYSLOG = NULL;

}}
