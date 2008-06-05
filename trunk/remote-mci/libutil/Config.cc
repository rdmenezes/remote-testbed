#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "libutil/Config.h"

namespace remote { namespace util {

using namespace remote::util;

class Config::Option
{
public:
	std::string name;
	std::string help;
	Config::type type;
	bool changed;
	union {
		std::string *string;
		uint16_t *uint16;
		uint64_t *uint64;
		bool *boolean;
		Option *alias;
	} value;

	Option(std::string name, std::string help, Config::type type)
		: name(name), help(help), type(type), changed(false)
	{ }
};

Config::Config(std::string program, std::string configFile)
	: program(program), configFile(configFile), options()
{ }

void Config::operator()(std::string name, bool *boolean, std::string help)
{
	add(name, help, Config::BOOL)->value.boolean = boolean;
}

void Config::operator()(std::string name, uint16_t *uint16, std::string help)
{
	add(name, help, Config::UINT16)->value.uint16 = uint16;
}

void Config::operator()(std::string name, uint64_t *uint64, std::string help)
{
	add(name, help, Config::UINT64)->value.uint64 = uint64;
}

void Config::operator()(std::string name, std::string *string, std::string help)
{
	add(name, help, Config::STRING)->value.string = string;
}

void Config::operator()(std::string name, std::string alias)
{
	Option *option = get(alias);

	if (!option)
		fprintf(stderr, "Option '%s' does not exist\n", name.c_str());
	else
		add(name, option->help, Config::ALIAS)->value.alias = option;
}

bool Config::read(int argc, char **argv)
{
	Option *config = add("config", "Path to config file", Config::STRING);

	add("config-file", "", Config::ALIAS)->value.alias = config;
	config->value.string = &configFile;

	add("help", "Print usage and exit", Config::HELP);
	add("version", "Print version and exit", Config::VERSION);

	for (int i = 1; i < argc; i++) {
		char *arg = argv[i];
		char *sep = strchr(arg, '=');
		std::string name;
		std::string value = sep ? sep + 1 : "";

		if (sep)
			*sep = 0;
		name = arg + strspn(arg, "-");
		if (arg == name) {
			fprintf(stderr, "%s is not an option\n", arg);
			return false;
		}

		if (!parseOption(name, value)) {
			fprintf(stderr, "Unknown option %s\n", arg);
			return false;
		}
	}

	if (!parseFile(configFile, config->changed))
		return false;

	return true;
}

static inline char *
sanitize(char *pos)
{
	int len;

	while (isspace(*pos))
		pos++;
	for (len = strlen(pos); len > 0; len--) {
		if (!isspace(pos[len - 1]))
			break;
		pos[len - 1] = 0;
	}

	return pos;
}

bool Config::parseFile(std::string path, bool fail)
{
	FILE *file = fopen(path.c_str(), "r");
	char buf[BUFSIZ];
	size_t lineno;

	if (!file) {
		if (!fail)
			return true;
		fprintf(stderr, "Failed to open %s: %s\n",
			path.c_str(), strerror(errno));
		return false;
	}

	for (lineno = 0; fgets(buf, sizeof(buf), file); lineno++) {
		char *pos = strchr(buf, '#');
		std::string name, value;

		if (pos)
			*pos = 0;

		pos = strchr(buf, '=');
		if (pos) {
			*pos++ = 0;
			value = sanitize(pos);
		}
		name = sanitize(buf);

		if (name == "") {
			if (value != "")
				fprintf(stderr, "Garbage on line %d\n", lineno);
			continue;
		}

		if (!parseOption(name, value)) {
			fprintf(stderr, "File %s line %d: Unknown option %s\n",
				path.c_str(), lineno, name.c_str());
			return false;
		}
	}
	fclose(file);
	return true;
}

Config::Option *Config::get(std::string name)
{
	std::list<Option *>::iterator iterator;

	for (iterator = options.begin(); iterator != options.end(); iterator++) {
		Option *option = *iterator;

		if (!strcasecmp(option->name.c_str(), name.c_str()))
			return option->type == Config::ALIAS
			     ? option->value.alias : option;
	}

	return NULL;
}

Config::Option *Config::add(std::string name, std::string help, Config::type type)
{
	Option *option = new Option(name, help, type);

	if (option)
		options.push_back(option);

	return option;
}

bool Config::parseOption(std::string name, std::string value)
{
	Option *option = get(name);

	if (!option)
		return false;

	switch (option->type) {
	case Config::VERSION:
		printf("%s %s\n", program.c_str(), PACKAGE_VERSION);
		exit(EXIT_SUCCESS);
		break;

	case Config::HELP:
		printHelp();
		exit(EXIT_SUCCESS);
		break;

	case Config::ALIAS:
		fprintf(stderr, "Config::get returned alias %s\n", value.c_str());
		break;

	case Config::BOOL:
		*option->value.boolean = (value == ""  || value == "true" ||
					  value == "1" || value == "yes");
		break;

	case Config::UINT16:
	case Config::UINT64:
	{
		const char *ptr = value.c_str();
		char *end;
		uint64_t num;

		errno = 0;
		num = strtoul(ptr, &end, 10);
		if (errno || *end) {
			fprintf(stderr, "Bogus value: %s\n", value.c_str());
			return false;
		}

		if (option->type == Config::UINT64) {
			*option->value.uint64 = num;
		} else if (num <= 65535) {
			*option->value.uint16 = (uint16_t) num; 
		} else {
			fprintf(stderr, "Value for %s too big\n", name.c_str());
			return false;
		}
		break;
	}
	case Config::STRING:
		if (value == "") {
			fprintf(stderr, "Missing value\n");
			return false;
		}
		if (value[0] == '"' && value[value.size() - 1] == '"')
			value = value.substr(1, value.size() - 2);
		if (value[0] == '\'' && value[value.size() - 1] == '\'')
			value = value.substr(1, value.size() - 2);
		*option->value.string = value;
		break;
	}

	option->changed = true;
	return true;
}

void Config::printHelp()
{
	std::list<Option *>::iterator iterator;

	printf("%s %s\n\n", program.c_str(), PACKAGE_VERSION);
	printf("Usage: %s [options]\n\n", program.c_str());
	printf("Options:\n");

	for (iterator = options.begin(); iterator != options.end(); iterator++) {
		Option *option = *iterator;
		std::string name = option->name;
		std::ostringstream help;

		help << option->help;

		switch (option->type) {
		case Config::ALIAS:
			continue;

		case Config::BOOL:
			help << " (";
			help << (*option->value.boolean ? "true" : "false");
			help << ")";
			break;

		case Config::UINT16:
			name += "=uint16";
			help << " (" << *option->value.uint16 << ")";
			break;

		case Config::UINT64:
			name += "=uint64";
			help << " (" << *option->value.uint64 << ")";
			break;

		case Config::STRING:
			name += "=string";
			help << " (";
			help << *option->value.string;
			help << ")";
			break;
		}

		printf(" --%-25s %s\n", name.c_str(), help.str().c_str());
	}
}

}}
