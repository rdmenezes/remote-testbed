#ifndef MOTEADDRESSES_H_
#define MOTEADDRESSES_H_
#include "types.h"
#include "MCIAddress.h"

namespace remote { namespace diku_mcs {

using namespace mcs;

/** This is the deployment specific specialization of the 
 * MCIAddress class. It has a TOS (TinyOS) address as
 * well as a MAC address for each mote **/
class MoteAddresses : public MCIAddress
{
	public:
		/** Constructor
		 * \param p_tosAddress TOS address
		 * \param p_macAddress MAC address
		**/
		MoteAddresses(uint16_t p_tosAddress,uint64_t p_macAddress);
		/** TOS address **/
		uint16_t tosAddress;
		/** MAC address **/
		uint64_t macAddress;
};

}}

#endif /*MOTEMACADDRESS_H_*/
