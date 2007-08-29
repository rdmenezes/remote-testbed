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
public class MsgUint32 extends MsgPrimitive {

	public MsgUint32() {
		super(4);
	}

	public MsgUint32(long value) {
		super(4);
		this.setValue(value);
	}

	public long getValue() {
		return (long)value.getInt(0);
	}

	public void setValue(long value) {
		this.value.putInt(0,(int)value);
	}
}
