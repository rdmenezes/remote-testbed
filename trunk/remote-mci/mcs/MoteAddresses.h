#ifndef MOTEADDRESSES_H_
#define MOTEADDRESSES_H_

#include <string>
#include "MCIAddress.h"

namespace remote { namespace mcs {

using namespace mcs;

/** This is the deployment specific specialization of the
 * MCIAddress class. It has a NET address and a MAC address
 * for each mote **/
class MoteAddresses : public MCIAddress
{
	public:
		/** Constructor
		 * \param p_netAddress NET address
		 * \param p_mac MAC address
		**/
		MoteAddresses(std::string p_mac, std::string p_netAddress = "");
		/** NET address **/
		std::string netAddress;
		/** MAC address **/
		std::string mac;
};

}}

#endif /*MOTEMACADDRESS_H_*/
