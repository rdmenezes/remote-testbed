#ifndef REMOTE_UTIL_LOG_H
#define REMOTE_UTIL_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

namespace remote { namespace util {

class Log {
	public:
		/** Setup logging
		 *
		 * Configures the various logging options and prepares
		 * for future calls to redirect messages to the
		 * requested place.
		 *
		 * The default log level can be one of the log level
		 * identifier defined by the <syslog.h> system header:
		 * LOG_EMERG, LOG_ERR, LOG_WARNING, LOG_INFO, or
		 * LOG_DEBUG.
		 *
		 * The log file is an ordinary file handle. It should be
		 * a file opened in append mode. To use syslog, pass NULL.
		 * The default is to use stdout.
		 *
		 * @param ident		A unique log identifier
		 * @param level		The default log level, messages
		 *			with log level of lower importance
		 *			than this will be discarded.
		 * @param file		The log file handle.
		 */
		static void open(const char *ident, int level = LOG_DEBUG,
				 FILE *file = stdout);

		/** Fatal error message
		 *
		 * An emergency condition occurred rendering the system
		 * unusable. When syslog is enabled this will map to a
		 * LOG_EMERG message.
		 */
		static void fatal(const char *format, ...);

		/** Error message
		 *
		 * An error condition occurred.
		 */
		static void error(const char *format, ...);

		/** Warning message
		 *
		 * An warning condition occurred.
		 */
		static void warn(const char *format, ...);

		/** Informational message
		 *
		 * A normal and non-significant condition occurred.
		 */
		static void info(const char *format, ...);

		/** Debug message
		 *
		 * For logging occurrences relevant during debuging.
		 */
		static void debug(const char *format, ...);

	private:
		static void put(int priority, const char *format, va_list params);
		static const char *log_ident;
		static FILE *log_file;
		static int log_level;

		static inline bool use_syslog()
		{
			return log_file == NULL;
		}

		/* This might be a bad hardcoded way to discard messages of
		 * less importance. However, the BSD syslog.h file used by
		 * glibc states that EMERG < ERR < WARNING < INFO < DEBUG. */
		static inline bool ignore(int priority)
		{
			return priority > log_level;
		}
};

}}

#endif
