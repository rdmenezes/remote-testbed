#include "MoteAddresses.h"

namespace remote { namespace diku_mcs {

MoteAddresses::MoteAddresses(std::string p_mac, uint16_t p_tosAddress)
              : tosAddress(p_tosAddress), mac(p_mac)
{
}

}}
