/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package remote.protocol;


/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MsgUint8 extends MsgPrimitive {

	public MsgUint8() {
		super(1);
	}

	public MsgUint8(short value) {
		super(1);
		this.setValue(value);
	}

	public short getValue() {
		return (short)value.get(0);
	}

	public void setValue(short value) {
		this.value.put(0,(byte)value);
	}
}
