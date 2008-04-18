/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package remote.protocol.motecontrol;

import java.io.DataInput;
import java.io.DataOutput;

import remote.protocol.MsgIO;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MsgConfirm extends MsgIO {

	private MsgCommand command;
	private MsgResult result;
	private MsgStatus status;

	public MsgConfirm()
	{
		command = new MsgCommand();
		result = new MsgResult();
		status = new MsgStatus();
	}
	/* (non-Javadoc)
	 * @see message.MsgIO#read(java.io.DataInputStream)
	 */
	public void read(DataInput is) throws Exception
	{
		command.read(is);
		result.read(is);
		status.read(is);
	}

	/* (non-Javadoc)
	 * @see message.MsgIO#write(java.io.DataOutputStream)
	 */
	public void write(DataOutput os) throws Exception
	{
		command.write(os);
		result.write(os);
		status.write(os);
	}
	public MsgCommand getCommand() {
		return command;
	}
	public MsgResult getResult() {
		return result;
	}
	public MsgStatus getStatus() {
		return status;
	}

}
