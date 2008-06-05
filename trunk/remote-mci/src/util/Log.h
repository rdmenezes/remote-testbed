#ifndef REMOTE_UTIL_LOG_H
#define REMOTE_UTIL_LOG_H

#include "remote.h"

namespace remote { namespace util {

class Log {
	public:
		/** Log level specifiers
		 *
		 * The following log levels can be used to specify the default
		 * log level when opening a log. They are listed in the order
		 * of priority starting with the most important. When
		 * specifying the log level, messages with higher priorities
		 * are automatically included, e.g. Log::INFO will include
		 * Log::ERROR and Log::FATAL.
		 *
		 * The specifiers all maps internally to syslog priorities as
		 * defined in <syslog.h>.
		 */
		enum {
			FATAL = LOG_EMERG,
			ERROR = LOG_ERR,
			WARN  = LOG_WARNING,
			INFO  = LOG_INFO,
			DEBUG = LOG_DEBUG,
		};

		/** Syslog specifier
		 *
		 * May be passed when opening a log, to specify that
		 * syslog should be used. E.g.:
		 *
		 * 	Log::open("remote-mch", Log::INFO, Log::SYSLOG);
		 */
		static const FILE *SYSLOG;

		/** Setup logging
		 *
		 * Configures the various logging options and prepares
		 * for future calls to redirect messages to the
		 * requested place.
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
		static void open(const char *ident, int level = Log::INFO,
				 const FILE *file = stdout);

		/** Fatal error message
		 *
		 * An emergency condition occurred rendering the system
		 * unusable.
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
		static const char *ident;
		static const FILE *file;
		static int level;

		static inline bool use_syslog()
		{
			return Log::file == SYSLOG;
		}

		/* This might be a bad hardcoded way to discard messages of
		 * less importance. However, the BSD syslog.h file used by
		 * glibc states that EMERG < ERR < WARNING < INFO < DEBUG. */
		static inline bool ignore(int priority)
		{
			return priority > Log::level;
		}
};

}}

#endif
