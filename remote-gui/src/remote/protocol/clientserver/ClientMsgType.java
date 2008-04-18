package remote.protocol.clientserver;
import remote.protocol.MsgUint8;
public class ClientMsgType extends MsgUint8 {
	public static final short SESSION = 0;
	public static final short CLIENTREQUEST = 1;
	public static final short CLIENTCONFIRM = 2;
}
