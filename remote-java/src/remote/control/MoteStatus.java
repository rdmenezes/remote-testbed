package remote.control;

/** Mote status type. */
public class MoteStatus {

	/** The status of the mote is unknown. */
	public static final MoteStatus UNKNOWN     = new MoteStatus(MoteStatusType.UNKNOWN);

	/** The mote is no longer available. */
	public static final MoteStatus UNAVAILABLE = new MoteStatus(MoteStatusType.UNAVAILABLE);

	/** The mote is powered off. */
	public static final MoteStatus STOPPED     = new MoteStatus(MoteStatusType.STOPPED);

	/** The mote is running. */
	public static final MoteStatus RUNNING     = new MoteStatus(MoteStatusType.RUNNING);

	/** The mote is being programmed. */
	public static final MoteStatus PROGRAMMING = new MoteStatus(MoteStatusType.PROGRAMMING);

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
