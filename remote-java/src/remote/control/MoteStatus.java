package remote.control;

/** Mote status type. */
public class MoteStatus {

	public static final MoteStatus UNKNOWN     = new MoteStatus(MoteStatusType.UNKNOWN);
	public static final MoteStatus UNAVAILABLE = new MoteStatus(MoteStatusType.UNAVAILABLE);
	public static final MoteStatus STOPPED     = new MoteStatus(MoteStatusType.STOPPED);
	public static final MoteStatus RUNNING     = new MoteStatus(MoteStatusType.RUNNING);
	public static final MoteStatus PROGRAMMING = new MoteStatus(MoteStatusType.PROGRAMMING);

	/* ... */
	public static final MoteStatus STOPPING    = new MoteStatus(MoteStatusType.STOPPING);
	public static final MoteStatus RESETTING   = new MoteStatus(MoteStatusType.RESETTING);
	public static final MoteStatus STARTING    = new MoteStatus(MoteStatusType.STARTING);
	public static final MoteStatus CANCELLING  = new MoteStatus(MoteStatusType.CANCELLING);

	private int status;

	public MoteStatus(int status)
	{
		this.status = status;
	}

	@Override
	public String toString()
	{
		if (this == UNAVAILABLE)
			return "unavailable";
		if (this == STOPPED)
			return "stopped";
		if (this == RUNNING)
			return "running";
		if (this == PROGRAMMING)
			return "programming";
		return "unknown";
	}

}
