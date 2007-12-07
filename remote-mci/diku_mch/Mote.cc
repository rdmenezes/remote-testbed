#include "Mote.h"

namespace remote { namespace diku_mch {

using namespace remote::util;

Mote::Mote(std::string& p_mac, std::string& p_directory)
	: SerialControl(), mac(p_mac), directory(p_directory)
{
	validate();
	Log::info("Mote %s (%s) @ %s", mac.c_str(), platform.c_str(), path.c_str());
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
	std::string oldTty = tty;

	isvalid = true;

	path = File::readFile(directory + "path");
	if (path == "")
		isvalid = false;

	platform = File::readFile(directory + "platform");
	if (platform == "")
		isvalid = false;

	tty = File::readLink(directory + "tty");
	if (tty == "")
		isvalid = false;

	if (!isOpen() && openTty() == FAILURE)
		isvalid = false;

	if (!isvalid)
		Log::warn("Mote %s @ %s is invalid", mac.c_str(), tty.c_str());
}

const std::string& Mote::getMac()
{
	return mac;
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
