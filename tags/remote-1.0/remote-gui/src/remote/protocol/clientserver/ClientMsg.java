package remote.protocol.clientserver;

import java.io.DataInput;
import java.io.DataOutput;
import remote.protocol.MsgIO;
import remote.protocol.MsgUint32;

public class ClientMsg extends MsgIO {
	private static final long currentProtocolVersion = 1;
	private MsgUint32 protocolVersion = new MsgUint32(1);
	private ClientMsgType type = new ClientMsgType();
	private MsgSession session = new MsgSession();
	private MsgClientRequest clientRequest = new MsgClientRequest();
	private MsgClientConfirm clientConfirm = new MsgClientConfirm();

	public ClientMsg() {
		super();
	}

	public void read(DataInput is) throws Exception
	{
		protocolVersion.read(is);
		type.read(is);
		switch(type.getValue())
		{
			case ClientMsgType.SESSION:
				session.read(is);
				break;
			case ClientMsgType.CLIENTCONFIRM:
				clientConfirm.read(is);
				break;
			case ClientMsgType.CLIENTREQUEST:
				clientRequest.read(is);
				break;
			default:
				break;
		}
	}

	public void write(DataOutput os) throws Exception
	{
		protocolVersion.setValue(ClientMsg.currentProtocolVersion);
		protocolVersion.write(os);
		type.write(os);
		switch(type.getValue())
		{
			case ClientMsgType.SESSION:
				session.write(os);
				break;
			case ClientMsgType.CLIENTCONFIRM:
				clientConfirm.write(os);
				break;
			case ClientMsgType.CLIENTREQUEST:
				clientRequest.write(os);
				break;
			default:
				break;
		}
	}

	public MsgSession getSession() {
		return session;
	}

	public MsgSession getAuthConfirm() {
		return session;
	}

	public MsgUint32 getProtocolVersion() {
		return protocolVersion;
	}

	public MsgClientConfirm getClientConfirm() {
		return clientConfirm;
	}

	public MsgClientRequest getClientRequest() {
		return clientRequest;
	}

	public ClientMsgType getType() {
		return type;
	}


}
