/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.protocols;

import java.io.DataInput;
import java.io.DataOutput;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MsgPayload extends MsgIO {

	private MsgUint32 length;
	private byte data[];

	public MsgPayload()
	{
		length = new MsgUint32();
	}

	/* (non-Javadoc)
	 * @see message.MsgIO#read(java.io.DataInputStream)
	 */
	public void read(DataInput is) throws Exception
	{
		length.read(is);
		if (length.getValue() > 0)
		{
			data = new byte[(int)length.getValue()];
			is.readFully(data);
		}
		else
		{
			data = null;
		}
	}

	/* (non-Javadoc)
	 * @see message.MsgIO#write(java.io.DataOutputStream)
	 */
	public void write(DataOutput os) throws Exception
	{
		if (data == null) length.setValue(0);
		else length.setValue(data.length);
		length.write(os);
		if (length.getValue() > 0) os.write(data,0,(int)length.getValue());
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

	public void setData(String s)
	{
		this.data = s.getBytes();
	}

	public MsgUint32 getLength() {
		if (data == null) length.getValue();
		else length.setValue(data.length);
		return length;
	}


}
