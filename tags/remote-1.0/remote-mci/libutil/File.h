#ifndef REMOTE_UTIL_FILE_H
#define REMOTE_UTIL_FILE_H

#include <string>

namespace remote { namespace util {

/** File utility methods.
 *
 * This class provides various file related utility methods.
 */
class File
{
public:
	/** Read whole file into string.
	 *
	 * @param filename	The name of the file to read.
	 * @return		The content of the file.
	 */
	static std::string readFile(std::string filename);

	/** Read destination of symbolic link.
	 *
	 * @param linkname	The name of the symbolic link to read.
	 * @return		The destination path of the symlink.
	 */
	static std::string readLink(std::string linkname);

	/** Write data to file.
	 *
	 * @param filename	The name of the file to write.
	 * @param data		The file content.
	 * @param datalen	The length of the content.
	 * @return		True if the whole file was written.
	 */
	static bool writeFile(std::string filename, const void *data, uint32_t datalen);

	/** Check if file exists.
	 *
	 * @param filename	The name of the file.
	 * @return		True if the file exists.
	 */
	static bool exists(std::string filename);
};

}}

#endif
