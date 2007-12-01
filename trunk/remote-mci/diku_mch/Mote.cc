#include "Mote.h"

namespace remote { namespace diku_mch {

using namespace remote::util;

Mote::Mote(std::string& p_mac, std::string& p_directory)
	: SerialControl(), mac(p_mac), directory(p_directory)
{
	validate();
	log("Mote %s (%s) at %s\n", mac.c_str(), platform.c_str(), path.c_str());
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

	path = File::readFile(directory + "path");
	if (path == "")
		isvalid = false;

	platform = File::readFile(directory + "platform");
	if (platform == "")
		isvalid = false;

	p_tty = File::readLink(directory + "tty");
	if (p_tty != tty) {
		log("Changed TTY from %s to %s\n", tty.c_str(), p_tty.c_str());
		tty = p_tty;

		if (isOpen())
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

const std::string& Mote::getPlatform()
{
	return platform;
}

std::string Mote::getImagePath()
{
	return directory + "image";
}

std::string Mote::getProgrammerPath()
{
	return directory + "programmer";
}

}}
