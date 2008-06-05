#include "mcs/MoteAddresses.h"

namespace remote { namespace mcs {

MoteAddresses::MoteAddresses(std::string p_mac, std::string p_netAddress)
              : netAddress(p_netAddress), mac(p_mac)
{
}

}}
