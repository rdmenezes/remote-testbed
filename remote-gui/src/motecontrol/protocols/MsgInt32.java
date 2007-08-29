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
class MsgInt32 extends MsgPrimitive {

	public MsgInt32() {
		super(4);
	}

	public MsgInt32(int value) {
		super(4);
		this.setValue(value);
	}

	public int getValue() {
		return (int)value.getInt(0);
	}

	public void setValue(int value) {
		this.value.putInt(0,value);
	}

}
