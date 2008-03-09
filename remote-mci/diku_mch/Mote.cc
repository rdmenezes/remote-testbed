#include "Mote.h"

namespace remote { namespace diku_mch {

using namespace remote::util;

Mote::Mote(std::string& p_mac, std::string& p_directory)
	: SerialControl(), mac(p_mac), directory(p_directory)
{
	validate();
	if (isvalid && !setupTty())
		isvalid = false;

	Log::info("Mote %s (%s) @ %s", mac.c_str(), platform.c_str(), path.c_str());
}

bool Mote::setupTty()
{
	if (!openTty())
		return false;

	return true;
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
	isvalid = true;
	imagefile = directory + "image";

	programmer = directory + "programmer";
	if (programmer == "" || !File::exists(programmer))
		isvalid = false;

	path = File::readFile(directory + "path");
	if (path == "")
		isvalid = false;

	platform = File::readFile(directory + "platform");
	if (platform == "")
		isvalid = false;

	tty = File::readLink(directory + "tty");
	if (tty == "")
		isvalid = false;

	if (!isvalid)
		Log::warn("Mote %s is invalid", mac.c_str());
}


result_t Mote::program(std::string tos, const uint8_t *image, uint32_t imagelen)
{
	if (hasChild())
		return FAILURE;

	if (File::writeFile(imagefile, image, imagelen)) {
		std::string mac_env = "macaddress=" + mac;
		std::string tos_env = "tosaddress=" + tos;
		std::string platform_env = "platform=" + platform;
		char * const args[] = {
			(char *) programmer.c_str(),
			(char *) tty.c_str(),
			(char *) imagefile.c_str(),
			NULL
		};
		char * const envp[] = {
			(char *) mac_env.c_str(),
			(char *) tos_env.c_str(),
			(char *) platform_env.c_str(),
			NULL
		};

		Log::info("Programming mote %s", mac.c_str());

		if (runChild(args, envp))
			return SUCCESS;

		remove(imagefile.c_str());
	}

	return FAILURE;
}

result_t Mote::cancelProgramming()
{
	return getChildResult(true);
}

result_t Mote::getChildResult(bool force)
{
	bool success = endChild(force);

	remove(imagefile.c_str());
	return success ? SUCCESS : FAILURE;
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

}}
