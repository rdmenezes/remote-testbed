package remote.control;

/** Low-level mote status type codes. */
public interface MoteStatusType {
	int UNKNOWN	= 0;
	int UNAVAILABLE = 1;
	int STOPPED	= 2;
	int RUNNING	= 3;
	int PROGRAMMING = 4;
	int STOPPING	= 5;
	int RESETTING	= 6;
	int STARTING	= 7;
	int CANCELING	= 8;
}
