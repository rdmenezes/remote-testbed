#ifndef _MOTECONTROLINFRASTRUCTURE_H_
#define _MOTECONTROLINFRASTRUCTURE_H_

#include "MsgPayload.h"
#include "MCIAddress.h"

namespace remote { namespace mcs {

using namespace protocols;
	
/** An object of this class manage mote infrastructure abstractions.
	Any number of MoteControlInfrastructure may be created and each may manage
	any number of motes;
	The MoteControlInfrastructure is responsible for registering hotplugged motes
	and for routing messages through the underlying network to the motes.
**/
class MoteControlInfrastructure
{
	public:
		/** Destructor **/
		virtual ~MoteControlInfrastructure()  {};
		/** Sends a request to the mote with the specied address
		 * \param address Control infrastructure address of the mote.
		 * \param request Request to send to the mote.
		**/
		virtual void request( MCIAddress& address, MsgPayload& request ) = 0;
};

}}

#endif
