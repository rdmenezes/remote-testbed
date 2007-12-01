#include "Mote.h"

namespace remote { namespace diku_mch {

Mote::Mote(std::string& p_mac, std::string& p_directory)
	: SerialControl(), mac(p_mac), directory(p_directory)
{
	validate();
	log("New mote %s at %s\n", mac.c_str(), path.c_str());
}


bool Mote::isValid()
{
	return isvalid;
}

void Mote::invalidate()
{
	isvalid = false;
}

void Mote::validate()
{
	std::string p_tty = directory + "tty";

	isvalid = true;

	path = readFile(directory + "path");
	if (path == "")
		isvalid = false;

	p_tty = readLink(directory + "tty");
	if (p_tty != tty) {
		log("Changed TTY from %s to %s\n", tty.c_str(), p_tty.c_str());
		tty = p_tty;

		if (isOpen)
			closeTty();
		if (openTty() == FAILURE)
			isvalid = false;
	}

	if (!isvalid)
		log("Mote at %s is invalid.\n", tty.c_str());
}

const std::string& Mote::getMac()
{
	return mac;
}

const std::string& Mote::getDirectory()
{
	return directory;
}

const std::string& Mote::getDevicePath()
{
	return path;
}

std::string Mote::readFile(std::string filename)
{
	char buffer[1024];
	ssize_t size, i;
	int fd;

	fd = open(filename.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd < 0)
		return std::string("");

	do {
		size = read(fd, buffer, sizeof(buffer) - 1);
	} while ((size < 0) && (errno == EAGAIN || errno == EINTR));

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

std::string Mote::readLink(std::string linkname)
{
	std::string filename;
	char buf[PATH_MAX];
	ssize_t buflen;
	int relative = 0;

	buflen = readlink(linkname.c_str(), buf, sizeof(buf));
	if (buflen == -1 || linkname.size() >= sizeof(buf))
		return std::string("");

	buf[buflen] = 0;
	filename = buf;

	while (filename.substr(0, 3) == "../") {
		filename.erase(0, 3);
		relative++;
	}

	if (!relative)
		return filename;

	linkname.copy(buf, linkname.size());
	do
		dirname(buf);
	while (relative--);

	filename.insert(0, "/");
	filename.insert(0, buf);

	return filename;
}

}}
