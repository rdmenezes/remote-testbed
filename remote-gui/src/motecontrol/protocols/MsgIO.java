/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.protocols;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
abstract public class MsgIO {
	public byte[] getBytes() throws Exception
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		DataOutputStream os = new DataOutputStream(bos);
		write(os);
		return bos.toByteArray();
	}

	public void setBytes(byte[] bytes) throws Exception
	{
		ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
		DataInputStream is = new DataInputStream(bis);
		read(is);
	}

	abstract public void read(DataInput is) throws Exception;

	abstract public void write(DataOutput os) throws Exception;
}
