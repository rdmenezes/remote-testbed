package remote.protocol.clientserver;

import remote.protocol.MsgUint8;

public class MsgClientCommand extends MsgUint8 {

	public static final short MSGCLIENTCOMMAND_GETMOTECONTROL = 0;
	public static final short MSGCLIENTCOMMAND_DROPMOTECONTROL = 1;
	public static final short MSGCLIENTCOMMAND_MOTEMESSAGE = 100;
}
