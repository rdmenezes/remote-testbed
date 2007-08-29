/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.protocols;


/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MsgUint16 extends MsgPrimitive {

	public MsgUint16() {
		super(2);
	}

	public MsgUint16(int value) {
		super(2);
		this.value.putShort(0,(short)value);
	}

	public int getValue() {
		return (int)value.getShort(0);
	}

	public void setValue(int value) {
		this.value.putShort(0,(short)value);
	}


}
