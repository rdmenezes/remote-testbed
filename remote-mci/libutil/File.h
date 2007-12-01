#ifndef REMOTE_UTIL_FILE_H
#define REMOTE_UTIL_FILE_H

#include <string>

namespace remote { namespace util {

class File
{
	public:
		static std::string readFile(std::string filename);
		static std::string readLink(std::string linkname);
		static bool writeFile(std::string filename, const void *data, uint32_t datalen);
};

}}

#endif
