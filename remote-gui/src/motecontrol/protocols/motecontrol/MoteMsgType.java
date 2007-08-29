package diku.distlab.motecontrolclientlib.protocols.motecontrol;
import diku.distlab.motecontrolclientlib.protocols.MsgUint8;

public class MoteMsgType extends MsgUint8 {
	public static final short REQUEST = 0;
	public static final short CONFIRM = 1;
	public static final short DATA = 2;
}
