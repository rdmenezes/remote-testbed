#include "Mote.h"

namespace remote { namespace diku_mch {

Mote::Mote(uint64_t p_macAddress, std::string& p_path, std::string& p_tty)
	: SerialControl(p_tty), macAddress(p_macAddress), isvalid(true)
{
	path = p_path;
	printf("New mote %s at %s\n", getMacStr(macAddress), path.c_str());
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

	if (p_tty != tty) {
		Mote *testmote = new Mote(macAddress, path, p_tty);

		testmote->_open();
		bool testOpen = testmote->isOpen;
		testmote->_close();
		delete testmote;

		if (testOpen) {
			printf("Changed TTY from %s to %s\n", tty.c_str(), p_tty.c_str());
			tty = p_tty;

			if (isOpen) {
				_close();
				_open();
			}
			isvalid = isOpen;
		}
	}

	if (!isvalid)
		printf("Mote at %s is invalid.\n", tty.c_str());
}

uint64_t Mote::getMac()
{
	return macAddress;
}

const std::string& Mote::getPath()
{
	return path;
}

}}
