package remote.protocol.clientserver;
import java.io.DataInput;
import java.io.DataOutput;

import remote.protocol.MsgIO;
import remote.protocol.MsgUint32;

public class MsgSession extends MsgIO {

	MsgUint32 sessionId = new MsgUint32();

	public MsgSession() {
		super();
		// TODO Auto-generated constructor stub
	}

	public void read(DataInput is) throws Exception {
		sessionId.read(is);
	}

	public void write(DataOutput os) throws Exception {
		sessionId.write(os);
	}

	public MsgUint32 getSessionId() {
		return sessionId;
	}


}
