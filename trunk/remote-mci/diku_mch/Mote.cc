#include "Mote.h"

namespace remote { namespace diku_mch {

Mote::Mote(std::string& p_mac, std::string& p_directory, std::string& p_path, std::string& p_tty)
	: SerialControl(), mac(p_mac), directory(p_directory)
{
	validate(p_path, p_tty);
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

void Mote::validate(std::string& p_path, std::string& p_tty)
{
	isvalid = true;
	if (p_path != path) {
		path = p_path;
	}

	p_tty = readLink(directory + "tty");
	if (p_tty != tty) {
		log("Changed TTY from %s to %s\n", tty.c_str(), p_tty.c_str());
		tty = p_tty;

		if (isOpen) {
			closeTty();
			openTty();
			isvalid = isOpen;
		}
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
