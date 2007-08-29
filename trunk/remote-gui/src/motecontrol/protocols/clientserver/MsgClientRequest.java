package diku.distlab.motecontrolclientlib.protocols.clientserver;

import java.io.DataInput;
import java.io.DataOutput;

import diku.distlab.motecontrolclientlib.protocols.MsgIO;
import diku.distlab.motecontrolclientlib.protocols.MsgPayload;

public class MsgClientRequest extends MsgIO {

	private MsgClientCommand command = new MsgClientCommand();
	private MsgMoteIdList moteIdList = new MsgMoteIdList();
	private MsgPayload moteMsg = new MsgPayload();

	public void read(DataInput is) throws Exception {
		command.read(is);
		moteIdList.read(is);
		if (command.getValue() == MsgClientCommand.MSGCLIENTCOMMAND_MOTEMESSAGE)
		{
			moteMsg.read(is);
		}
	}

	public void write(DataOutput os) throws Exception {
		command.write(os);
		moteIdList.write(os);
		if (command.getValue() == MsgClientCommand.MSGCLIENTCOMMAND_MOTEMESSAGE)
		{
			moteMsg.write(os);
		}
	}

	public MsgPayload getMoteMsg() {
		return moteMsg;
	}

	public void setMoteMsg(MsgPayload moteMsg) {
		this.moteMsg = moteMsg;
	}

	public MsgClientCommand getCommand() {
		return command;
	}

	public MsgMoteIdList getMoteIdList() {
		return moteIdList;
	}


}
