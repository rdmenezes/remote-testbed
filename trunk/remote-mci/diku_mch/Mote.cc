#include "Mote.h"

namespace remote { namespace diku_mch {

Mote::Mote(std::string& p_mac, std::string& p_path, std::string& p_tty)
	: SerialControl(p_tty), mac(p_mac), path(p_path), isvalid(true)
{
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

	if (p_tty != tty) {
		Mote *testmote = new Mote(mac, path, p_tty);

		testmote->openTty();
		bool testOpen = testmote->isOpen;
		testmote->closeTty();
		delete testmote;

		if (testOpen) {
			log("Changed TTY from %s to %s\n", tty.c_str(), p_tty.c_str());
			tty = p_tty;

			if (isOpen) {
				closeTty();
				openTty();
			}
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

const std::string& Mote::getPath()
{
	return path;
}

}}
