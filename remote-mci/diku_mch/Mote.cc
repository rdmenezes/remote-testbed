#include "Mote.h"

namespace remote { namespace diku_mch {

Mote::Mote( uint64_t p_macAddress,
	    std::string& p_path,
	    uint16_t p_ttyNum )
	      : SerialControl(p_ttyNum),
		macAddress(p_macAddress),
		ttyNum(p_ttyNum),
		isvalid(true)
{
	path = p_path;
	printf("New mote %x at %s\n",(uint32_t)macAddress,path.c_str());
}


bool Mote::isValid()
{
	return isvalid;
}

void Mote::invalidate()
{
	isvalid = false;
}

void Mote::validate(std::string& p_path, uint16_t p_ttyNum)
{
	isvalid = true;
	if ( p_path != path )
	{
		path = p_path;
	}

	if ( p_ttyNum != ttyNum )
	{
		Mote* testmote = new Mote(macAddress,path,p_ttyNum);
		testmote->_open();
		bool testOpen = testmote->isOpen;
		testmote->_close();
		delete testmote;
		if ( testOpen )
		{
			printf("Changed ttynum from %u to %u\n",ttyNum,p_ttyNum);
			ttyNum = p_ttyNum;
			char nstring[100];
			sprintf(nstring,Configuration::vm["usbSerialDevicePath"].as<std::string>().c_str(),ttyNum);
			DeviceName = nstring;

			if (isOpen)
			{
				_close();
				_open();
			}
			isvalid = isOpen;
		}
	}
	if (!isvalid) printf("Mote at ttyUSB%u is invalid.\n",ttyNum);
}

uint64_t Mote::getMac()
{
	return macAddress;
}

uint16_t Mote::getTty()
{
	return ttyNum;
}

const std::string& Mote::getPath()
{
	return path;
}

}}
