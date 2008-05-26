package remote.service.motecontrol;

import java.io.Serializable;
import remote.control.MoteResult;
import remote.control.MoteStatus;

/** Mote control command result container. */
public final class CommandResult implements Serializable {

	/** Result of the executed command. */
	private MoteResult result;

	/** Mote status after the executed command. */
	private MoteStatus status;

	public CommandResult()
	{
		result = new MoteResult(MoteResult.SUCCESS);
		status = new MoteStatus(MoteStatus.UNKNOWN);
	}

	public CommandResult(MoteResult result, MoteStatus status)
	{
		setResult(result);
		setStatus(status);
	}

	public MoteResult getResult()
	{
		return result;
	}

	public void setResult(MoteResult result)
	{
		this.result = result;
	}

	public MoteStatus getStatus()
	{
		return status;
	}

	public void setStatus(MoteStatus status)
	{
		this.status = status;
	}

}
