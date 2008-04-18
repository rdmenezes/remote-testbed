package remote.protocol.motecontrol;
import java.io.DataInput;
import java.io.DataOutput;

import remote.protocol.MsgIO;
import remote.protocol.MsgPayload;
import remote.protocol.MsgUint32;

public class MoteMsg extends MsgIO {

	MsgUint32 protocolVersion = new MsgUint32();
	MoteMsgType type = new MoteMsgType();
	MsgRequest request = new MsgRequest();
	MsgConfirm confirm = new MsgConfirm();
	MsgPayload data = new MsgPayload();
	public void read(DataInput is) throws Exception {
		protocolVersion.read(is);
		type.read(is);
		switch(type.getValue())
		{
			case MoteMsgType.CONFIRM:
				confirm.read(is);
				break;
			case MoteMsgType.DATA:
				data.read(is);
				break;
			case MoteMsgType.REQUEST:
				request.read(is);
				break;
		}
	}

	public void write(DataOutput os) throws Exception
	{
		protocolVersion.write(os);
		type.write(os);
		switch(type.getValue())
		{
			case MoteMsgType.CONFIRM:
				confirm.write(os);
				break;
			case MoteMsgType.DATA:
				data.write(os);
				break;
			case MoteMsgType.REQUEST:
				request.write(os);
				break;
		}
	}

	public MsgConfirm getConfirm() {
		return confirm;
	}

	public MsgPayload getData() {
		return data;
	}

	public MsgUint32 getProtocolVersion() {
		return protocolVersion;
	}

	public MsgRequest getRequest() {
		return request;
	}

	public MoteMsgType getType() {
		return type;
	}


}
