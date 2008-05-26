package remote.control;

/** Mote command result container. */
public class MoteResult {
	public final static int FAILURE = 0;
	public final static int SUCCESS = 1;
	public final static int WORKING = 2;
	public final static int CANCELLED = 3;

	int result;

	public MoteResult(int result)
	{
		this.result = result;
	}

	int getResult()
	{
		return result;
	}
}
