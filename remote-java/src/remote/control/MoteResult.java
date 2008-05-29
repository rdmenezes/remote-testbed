package remote.control;

/** Mote command result.
 *
 * This class wraps the low-level result codes
 * by providing convenient symbols that can
 * be used for comparison.
 */
public class MoteResult {

	/** The result is success. */
	public final static MoteResult SUCCESS = new MoteResult(MoteResultType.SUCCESS);

	/** The result is failure. */
	public final static MoteResult FAILURE = new MoteResult(MoteResultType.FAILURE);

	/** The command is not supported. */
	public final static MoteResult NOT_SUPPORTED = new MoteResult(MoteResultType.NOT_SUPPORTED);

	/** The primitive result code. */
	private int result;

	public MoteResult(int result)
	{
		this.result = result;
	}

	public int getResult()
	{
		return result;
	}

}
