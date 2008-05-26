package remote.control;

/** Mote status container. */
public class MoteStatus {
	public static final int UNKNOWN = 0;
	public static final int UNAVAILABLE = 1;
	public static final int STOPPED = 2;
	public static final int RUNNING = 3;
	public static final int PROGRAMMING = 4;

	/* ... */
	public static final int STOPPING = 5;
	public static final int RESETTING = 6;
	public static final int STARTING = 7;

	int status;

	public MoteStatus(int status)
	{
		this.status = status;
	}

	public int getStatus()
	{
		return status;
	}

	public void setStatus(int newStatus)
	{
		switch (status) {
		case UNAVAILABLE:
		case STOPPED:
		case RUNNING:
		case PROGRAMMING:
		case UNKNOWN:
			status = newStatus;
		}
	}

	@Override
	public String toString()
	{
		switch (status) {
		case UNAVAILABLE:
			return "unavailable";
		case STOPPED:
			return "stopped";
		case RUNNING:
			return "running";
		case PROGRAMMING:
			return "programming";
		default:
			return "unknown";
		}
	}
}
