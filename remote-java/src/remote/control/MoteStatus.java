package remote.control;

/** Mote status type. */
public class MoteStatus {

	/** The status of the mote is unknown. */
	public static final MoteStatus UNKNOWN     = new MoteStatus(MoteStatusType.UNKNOWN, "unknown");

	/** The mote is no longer available. */
	public static final MoteStatus UNAVAILABLE = new MoteStatus(MoteStatusType.UNAVAILABLE, "unavailable");

	/** The mote is powered off. */
	public static final MoteStatus STOPPED     = new MoteStatus(MoteStatusType.STOPPED, "stopped");

	/** The mote is running. */
	public static final MoteStatus RUNNING     = new MoteStatus(MoteStatusType.RUNNING, "running");

	/** The mote is being programmed. */
	public static final MoteStatus PROGRAMMING = new MoteStatus(MoteStatusType.PROGRAMMING, "programming");

	private final int status;
	private final String string;

	private MoteStatus(int status, String string)
	{
		this.status = status;
		this.string = string;
	}

	@Override
	public String toString()
	{
		return string;
	}

}
