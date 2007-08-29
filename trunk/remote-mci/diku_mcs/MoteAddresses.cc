#include "MoteAddresses.h"

namespace remote { namespace diku_mcs {
	
MoteAddresses::MoteAddresses(uint16_t p_tosAddress,uint64_t p_macAddress)
              : tosAddress(p_tosAddress), macAddress(p_macAddress)
{
}

}}
