package remote.protocol;

import java.io.DataInput;
import java.io.DataOutput;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

abstract public class MsgPrimitive extends MsgIO {

	protected static ByteOrder byteOrder = ByteOrder.BIG_ENDIAN;
	protected ByteBuffer value;

	public MsgPrimitive(int bytes) {
		super();
		value = ByteBuffer.allocate(bytes);
		value.order(byteOrder);
	}

	public void read(DataInput is) throws Exception
	{
		is.readFully(value.array());
	}

	public void write(DataOutput os) throws Exception
	{
		os.write(value.array());
	}

}
