#ifndef REMOTE_PROTOCOLS_CLIENT_SERVER_LOCALCONSTANTS_H
#define REMOTE_PROTOCOLS_CLIENT_SERVER_LOCALCONSTANTS_H

enum result_types
{
	// operation results
	SUCCESS,
	FAILURE,
	MOTE_NOT_CONTROLLED,
	MOTE_NOT_FOUND,		/* Not currently used. */
	MOTE_OCCUPIED,		/* Not currently used. */
	NOT_SUPPORTED
};

#endif
