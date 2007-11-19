#include "MoteAddresses.h"

namespace remote { namespace diku_mcs {

MoteAddresses::MoteAddresses(uint16_t p_tosAddress, std::string p_mac)
              : tosAddress(p_tosAddress), mac(p_mac)
{
}

}}
