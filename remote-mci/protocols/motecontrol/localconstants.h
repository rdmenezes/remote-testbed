#ifndef LOCALCONSTANTS_H_
#define LOCALCONSTANTS_H_

namespace remote { namespace protocols { namespace motecontrol {

enum 
{
	// operation results
	SUCCESS,
	FAILURE,
	NOT_SUPPORTED
};

enum 
{
	MOTE_STATUS_UNKNOWN, // the status of the mote could not be determined
	MOTE_UNAVAILABLE, // mote is unreachable
	MOTE_STOPPED, // mote has been stopped
	MOTE_RUNNING, // mote is running
	MOTE_PROGRAMMING // Mote is being programmed
};

}}}

#endif /*LOCALCONSTANTS_H_*/