package diku.distlab.motecontrolclientlib.protocols.clientserver;
import diku.distlab.motecontrolclientlib.protocols.MsgUint8;
public class ClientMsgType extends MsgUint8 {
	public static final short SESSION = 0;
	public static final short CLIENTREQUEST = 1;
	public static final short CLIENTCONFIRM = 2;
}
