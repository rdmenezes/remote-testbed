#include "Mote.h"

namespace remote { namespace mch {

using namespace remote::util;

const std::string Mote::NONE = "none";
const std::string Mote::START = "start";
const std::string Mote::STOP = "stop";
const std::string Mote::RESET = "reset";
const std::string Mote::PROGRAM = "program";

Mote::Mote(std::string& p_mac, std::string& p_directory)
	: SerialControl(), mac(p_mac), directory(p_directory), running(false),
	  controlCmd(NONE)
{
	validate();
	if (isvalid && !setupTty(STOP))
		isvalid = false;

	Log::info("Mote %s (%s) @ %s", mac.c_str(), platform.c_str(), path.c_str());
}

bool Mote::setupTty(const std::string cmd)
{
	if (!openTty(platform, ttyData))
		return false;

	if (cmd == START || cmd == STOP || cmd == RESET)
		return power(cmd);

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
	if (!File::exists(programmer))
		isvalid = false;

	controller = directory + "controller";
	if (!File::exists(controller))
		controller = "";

	path = File::readFile(directory + "path");
	if (path == "")
		isvalid = false;

	platform = File::readFile(directory + "platform");
	if (platform == "")
		isvalid = false;

	ttyData = File::readLink(directory + "tty/data");
	if (ttyData == "")
		isvalid = false;

	ttyControl = File::readLink(directory + "tty/control");
	if (ttyControl == "")
		isvalid = false;

	if (!isvalid)
		Log::warn("Mote %s is invalid", mac.c_str());
}


bool Mote::start()
{
	return power(START);
}

bool Mote::stop()
{
	return power(STOP);
}

bool Mote::reset()
{
	return power(RESET);
}

bool Mote::power(const std::string cmd)
{
	bool resetting = cmd == RESET;

	if (!isOpen())
		return false;

	if (controller != "") {
		std::string platform_env = "platform=" + platform;
		char * const args[] = {
			(char *) controller.c_str(),
			(char *) ttyControl.c_str(),
			(char *) cmd.c_str(),
			NULL
		};
		char * const envp[] = {
			(char *) platform_env.c_str(),
			NULL
		};

		Log::info("Control mote %s to %s", mac.c_str(), cmd.c_str());

		if (runChild(args, envp)) {
			controlCmd = cmd;
			return true;
		}

	} else if (!resetting || controlDTR(running)) {
		bool enable = resetting ? !running : cmd == STOP;

		if (controlDTR(enable)) {
			running = !enable;
			return true;
		}

		/* Mirror that the first reset DTR change succeeded. */
		if (resetting)
			running = !running;
	}

	Log::error("Failed to %s mote %s: %s",
		   cmd.c_str(), mac.c_str(), strerror(errno));
	closeTty();
	return false;
}


bool Mote::program(std::string net, const uint8_t *image, uint32_t imagelen)
{
	if (hasChild())
		return false;

	if (File::writeFile(imagefile, image, imagelen)) {
		std::string mac_env = "macaddress=" + mac;
		std::string net_env = "netaddress=" + net;
		std::string platform_env = "platform=" + platform;
		char * const args[] = {
			(char *) programmer.c_str(),
			(char *) ttyControl.c_str(),
			(char *) imagefile.c_str(),
			NULL
		};
		char * const envp[] = {
			(char *) mac_env.c_str(),
			(char *) net_env.c_str(),
			(char *) platform_env.c_str(),
			NULL
		};

		Log::info("Programming mote %s", mac.c_str());

		if (runChild(args, envp)) {
			controlCmd = PROGRAM;
			return true;
		}

		remove(imagefile.c_str());
	}

	return false;
}

bool Mote::cancelProgramming()
{
	if (getControlCommand() != PROGRAM)
		return false;

	return getChildResult(true);
}

bool Mote::getChildResult(bool force)
{
	bool success = endChild(force);
	bool afterProgramming = controlCmd == PROGRAM;
	const std::string cmd = afterProgramming ? STOP : NONE;

	if (success) {
		if (controlCmd == START || controlCmd == RESET)
			running = true;
		else if (controlCmd == STOP)
			running = false;
	}

	if (afterProgramming)
		remove(imagefile.c_str());
	if (!setupTty(cmd))
		success = false;

	return success;
}


bool Mote::isRunning()
{
	return running;
}

const std::string& Mote::getControlCommand()
{
	return hasChild() ? controlCmd : NONE;
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
