#include "MoteAddresses.h"

namespace remote { namespace diku_mcs {

MoteAddresses::MoteAddresses(std::string p_mac, std::string p_tosAddress)
              : tosAddress(p_tosAddress), mac(p_mac)
{
}

}}
