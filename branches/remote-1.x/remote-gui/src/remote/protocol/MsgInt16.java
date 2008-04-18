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
class MsgInt16 extends MsgPrimitive {

	public MsgInt16() {
		super(2);
		value.order(byteOrder);
	}

	public MsgInt16(short value) {
		super(2);
		this.setValue(value);
	}

	public short getValue() {
		return value.getShort(0);
	}

	public void setValue(short value) {
		this.value.putShort(0,value);
	}

}
