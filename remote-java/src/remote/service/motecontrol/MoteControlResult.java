package remote.service.motecontrol;

import java.io.Serializable;
import remote.control.MoteResult;
import remote.control.MoteStatus;

/** Mote control command result container.
 *
 * This class is a container for the result and status information
 * that the mote control server returns when executing a command.
 */
public class MoteControlResult implements Serializable {

	/** Result of the executed command. */
	private MoteResult result;

	/** Mote status after the executed command. */
	private MoteStatus status;

	/** Default constructor.
	 *
	 * Sets result to {@link MoteResult#SUCCESS} and
	 * status to {@link MoteStatus#UNKNOWN}.
	 */
	public MoteControlResult()
	{
		this(MoteResult.SUCCESS, MoteStatus.UNKNOWN);
	}

	/** Constructor to set all fields.
	 *
	 * @param result	The mote control result.
	 * @param status	The resulting mote status.
	 */
	public MoteControlResult(MoteResult result, MoteStatus status)
	{
		setResult(result);
		setStatus(status);
	}

	/** Get result.
	 *
	 * @return		The mote control result.
	 */
	public MoteResult getResult()
	{
		return result;
	}

	/** Set mote control result.
	 *
	 * @param result	The result.
	 */
	public void setResult(MoteResult result)
	{
		this.result = result;
	}

	/** Get mote status.
	 *
	 * @return		The mote status.
	 */
	public MoteStatus getStatus()
	{
		return status;
	}

	/** Set status.
	 *
	 * @param status	The status.
	 */
	public void setStatus(MoteStatus status)
	{
		this.status = status;
	}

}
