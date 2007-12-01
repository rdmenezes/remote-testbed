/* Put log messages either to stdout or syslog.
 *
 * Assimilated from git.git::daemon.c on 2007-05-19. */

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "libutil/Log.h"

namespace remote { namespace util {

void
Log::open(const char *ident, int level, FILE *file)
{
	if (use_syslog()) {
		closelog();
	}

	log_ident = ident;
	log_level = level;
	log_file = file;

	if (use_syslog()) {
		openlog(log_ident, 0, LOG_DAEMON);
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
	fprintf(log_file, "%s - %s %s\n", strsep(&timestr, "\n"), log_ident, buf);
}

void
Log::error(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(LOG_ERR, format, params);
	va_end(params);
}

void
Log::warn(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(LOG_WARNING, format, params);
	va_end(params);
}

void
Log::info(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(LOG_INFO, format, params);
	va_end(params);
}

void
Log::debug(const char *format, ...)
{
	va_list params;

	va_start(params, format);
	put(LOG_DEBUG, format, params);
	va_end(params);
}

const char *Log::log_ident;
FILE *Log::log_file;
int Log::log_level;

}}
