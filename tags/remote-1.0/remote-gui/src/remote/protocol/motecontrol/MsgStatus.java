package remote.protocol.motecontrol;

import remote.protocol.MsgUint8;

public class MsgStatus extends MsgUint8 {

	public static final int UNKNOWN = 0;
	public static final int UNAVAILABLE = 1;
	public static final int STOPPED = 2;
	public static final int RUNNING = 3;
	public static final int PROGRAMMING = 4;
}
