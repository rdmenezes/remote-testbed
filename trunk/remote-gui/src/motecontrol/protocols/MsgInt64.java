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
public class MsgInt64 extends MsgPrimitive {

	public MsgInt64() {
		super(8);
	}

	public MsgInt64(long value) {
		super(8);
		this.setValue(value);
	}

	public long getValue() {
		return value.getLong(0);
	}

	public void setValue(long value) {
		this.value.putLong(0,value);
	}

}
