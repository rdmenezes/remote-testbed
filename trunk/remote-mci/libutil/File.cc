#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>

#include "libutil/File.h"
#include "libutil/Log.h"

namespace remote { namespace util {

std::string File::readFile(std::string filename)
{
	char buffer[1024];
	ssize_t size, i;
	int fd;

	fd = open(filename.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		Log::error("open(%s) failed: %s", filename.c_str(), strerror(errno));
		return std::string("");
	}

	do {
		size = read(fd, buffer, sizeof(buffer) - 1);
	} while ((size < 0) && (errno == EAGAIN || errno == EINTR));

	if (size == -1) {
		Log::error("read(%s) failed: %s", filename.c_str(), strerror(errno));
		size = 0;
	}

	/* Non-blocking, so we should have the full contents now. */
	close(fd);

	/* Sanitize the string to only hold sane ASCII characters. */
	for (i = size - 1; i >= 0; i--) {
		int c = buffer[i];

		if (c <= ' ' || c > 126) {
			/* Discard cruft at the end. */
			if (i + 1 == size) {
				size--;
				continue;
			}

			c = '_';
		} else if (c == '\'' || c == '"' || c == '\\') {
			c = '_';
		}

		buffer[i] = c;
	}
	buffer[size] = 0;

	return std::string(buffer);
}

std::string File::readLink(std::string linkname)
{
	std::string filename;
	char buf[PATH_MAX];
	ssize_t buflen;
	int relative = 0;

	buflen = readlink(linkname.c_str(), buf, sizeof(buf));
	if (buflen == -1) {
		Log::error("readlink(%s) failed: %s", linkname.c_str(), strerror(errno));
		return std::string("");
	}

	buf[buflen] = 0;
	filename = buf;

	while (filename.substr(0, 3) == "../") {
		filename.erase(0, 3);
		relative++;
	}

	if (!relative)
		return filename;

	if (linkname.size() >= sizeof(buf)) {
		Log::error("Link name too long: %s", linkname.c_str());
		return std::string("");
	}

	linkname.copy(buf, linkname.size());
	do
		dirname(buf);
	while (relative--);

	filename.insert(0, "/");
	filename.insert(0, buf);

	return filename;
}

bool File::writeFile(std::string filename, const void *data, uint32_t datalen)
{
	ssize_t filesize;
	int fd = open(filename.c_str(), O_CREAT | O_TRUNC | O_WRONLY);

	if (fd < 0) {
		Log::error("open(%s) failed: %s", filename.c_str(), strerror(errno));
		return false;
	}

	filesize = write(fd, data, datalen);
	if (fd < 0)
		Log::error("write(%s) failed: %s", filename.c_str(), strerror(errno));

	close(fd);
	if (filesize == (ssize_t) datalen)
		return true;

	remove(filename.c_str());
	return false;
}

}}
