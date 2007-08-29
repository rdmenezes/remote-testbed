package diku.distlab.motecontrolclientlib.protocols.motecontrol;

import java.io.DataInput;
import java.io.DataOutput;

import diku.distlab.motecontrolclientlib.protocols.MsgIO;
import diku.distlab.motecontrolclientlib.protocols.MsgPayload;

public class MsgRequest extends MsgIO {

	private MsgCommand command;
	private MsgPayload flashImage;

	public MsgRequest() {
		super();
		command = new MsgCommand();
		flashImage = new MsgPayload();
	}

	public void read(DataInput is) throws Exception
	{
		command.read(is);

		switch(command.getValue())
		{
			case MsgCommand.PROGRAM:
				flashImage.read(is);
				break;
			case MsgCommand.RESET:
			case MsgCommand.START:
			case MsgCommand.STOP:
			case MsgCommand.STATUS:
				break;
			default:
				throw new Exception("Invalid command in request!");
		}
	}

	public void write(DataOutput os) throws Exception
	{
		switch(command.getValue())
		{
			case MsgCommand.PROGRAM:
				command.write(os);
				flashImage.write(os);
				break;
			case MsgCommand.RESET:
			case MsgCommand.START:
			case MsgCommand.STOP:
			case MsgCommand.STATUS:
			case MsgCommand.CANCELPROGRAMMING:
				command.write(os);
				break;
			default:
				throw new Exception("Invalid command in request!");
		}
	}

	public MsgCommand getCommand() {
		return command;
	}

	public MsgPayload getFlashImage() {
		return flashImage;
	}


}