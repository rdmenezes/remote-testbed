/*
 * Created on Sep 28, 2005
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
public class MsgCommand extends MsgUint8 {
	public static final short PROGRAM = 0;
	public static final short STATUS = 1;
	public static final short RESET = 2;
	public static final short STOP = 3;
	public static final short START = 4;
	public static final short CANCELPROGRAMMING = 5;
}
