/*
 * Created on Sep 19, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.protocols.motecontrol;
import diku.distlab.motecontrolclientlib.protocols.MsgUint8;
/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MsgResult extends MsgUint8 {
	public static final int SUCCESS = 0;
	public static final int FAILURE = 1;
	public static final int NOT_SUPPORTED = 2;
}
