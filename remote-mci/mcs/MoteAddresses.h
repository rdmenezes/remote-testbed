#ifndef MOTEADDRESSES_H_
#define MOTEADDRESSES_H_

#include <string>

namespace remote { namespace mcs {

using namespace mcs;

/** This is a container for the infrastructure addresses. **/
class MoteAddresses
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
