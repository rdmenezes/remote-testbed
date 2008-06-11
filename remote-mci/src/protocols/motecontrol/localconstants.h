#ifndef REMOTE_PROTOCOLS_MOTECONTROL_LOCALCONSTANTS_H
#define REMOTE_PROTOCOLS_MOTECONTROL_LOCALCONSTANTS_H

namespace remote { namespace protocols { namespace motecontrol {

enum
{
	// operation results
	SUCCESS,
	FAILURE,
	NOT_SUPPORTED
};

typedef uint8_t status_t;
enum
{
	MOTE_STATUS_UNKNOWN, // the status of the mote could not be determined
	MOTE_UNAVAILABLE, // mote is unreachable
	MOTE_STOPPED, // mote has been stopped
	MOTE_RUNNING, // mote is running
	MOTE_PROGRAMMING // Mote is being programmed
};

}}}

#endif
