#ifndef REMOTE_UTIL_CONFIG_H
#define REMOTE_UTIL_CONFIG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#undef VERSION

#include <list>

namespace remote { namespace util {

/** Configuration handling.
 *
 * This class reads options from the command line and from a
 * configuration file.
 */
class Config
{
public:
	/** Option value type. */
	enum type {
		ALIAS,	/**< Alias option type. */
		BOOL,	/**< Boolean option type. */
		STRING,	/**< String option type. */
		UINT16,	/**< Unsigned 16-bit integer option type. */
		UINT64,	/**< Unsigned 64-bit integer option type. */
		HELP,	/**< Special type for handling the --help option. */
		VERSION	/**< Special type for handling the --version option. */
	};

	/** Initial the configuration module for a program
	 *
	 * @param program	The program name.
	 * @param configFile	The default path to the configuration file.
	 */
	Config(std::string program, std::string configFile);

	/** Add boolean option.
	 *
	 * @param name	The option name.
	 * @param value Reference to the option value.
	 * @param help	A short option description.
	 */
	void operator()(std::string name, bool *value, std::string help);

	/** Add uint16 option.
	 *
	 * @param name	The option name.
	 * @param value Reference to the option value.
	 * @param help	A short option description.
	 */
	void operator()(std::string name, uint16_t *value, std::string help);

	/** Add uint64 option.
	 *
	 * @param name	The option name.
	 * @param value Reference to the option value.
	 * @param help	A short option description.
	 */
	void operator()(std::string name, uint64_t *value, std::string help);

	/** Add string option.
	 *
	 * @param name	The option name.
	 * @param value Reference to the option value.
	 * @param help	A short option description.
	 */
	void operator()(std::string name, std::string *value, std::string help);

	/** Add alias to an existing option.
	 *
	 * @param name	The name of the alias.
	 * @param alias The name of the aliased option.
	 */
	void operator()(std::string name, std::string alias);

	/** Read options from command line and configuration file.
	 *
	 * @param argc	Number of command line arguments.
	 * @param argv	String array of command line arguments.
	 * @return	True if reading options succeeded.
	 */
	bool read(int argc, char **argv);

private:
	class Option;

	/** Read options from file.
	 *
	 * @param name	Name of configuration file.
	 * @param fail	Whether to error out if opening the file fails.
	 * @return	True if parsing the file succeeded.
	 */
	bool parseFile(std::string name, bool fail);

	/** Parse option name and value.
	 *
	 * @param name	The option name.
	 * @param value	The option value to parse.
	 * @return	True if parsing the option succeeded.
	 */
	bool parseOption(std::string name, std::string value);

	/** Print usage help and option descriptions. */
	void printHelp();

	/** Lookup an option by name.
	 *
	 * @param name	Name of the option to get.
	 * @return	The matching option or NULL if no option matched.
	 */
	Option *get(std::string name);

	/** Lookup an option by name.
	 *
	 * @param name	Name of the option to add.
	 * @param help	A short option description.
	 * @param type	The type of the option.
	 * @return	The newly added option or NULL.
	 */
	Option *add(std::string name, std::string help, enum type type);

	std::list<Option *> options;	/**< List of known options. */
	std::string program;		/**< The program name. */
	std::string configFile;		/**< The config file option string. */
};

}}

#endif
