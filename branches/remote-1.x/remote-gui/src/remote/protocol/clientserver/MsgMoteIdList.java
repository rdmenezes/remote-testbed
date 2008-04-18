package remote.protocol.clientserver;

import java.io.DataInput;
import java.io.DataOutput;
import java.util.ArrayList;

import remote.protocol.MsgIO;
import remote.protocol.MsgUint16;
import remote.protocol.MsgUint32;

public class MsgMoteIdList extends MsgIO {

	ArrayList moteIds;



	public MsgMoteIdList() {
		super();
		moteIds = new ArrayList();
	}

	public void read(DataInput is) throws Exception
	{
		MsgUint16 count = new MsgUint16();
		MsgUint32 moteId;
		clear();
		count.read(is);
		for (int i=0; i<count.getValue(); i++)
		{
			moteId = new MsgUint32();
			moteId.read(is);
			this.addMoteId(moteId);
		}
	}

	public void write(DataOutput os) throws Exception
	{
		MsgUint16 count = new MsgUint16(moteIds.size());
		MsgUint32 moteId;
		count.write(os);
		for (int i=0; i<count.getValue(); i++)
		{
			moteId = (MsgUint32)moteIds.get(i);
			moteId.write(os);
		}
	}

	public void clear()
	{
		moteIds.clear();
	}

	public int getLength()
	{
		return moteIds.size();
	}

	public void addMoteId(MsgUint32 moteId)
	{
		moteIds.add(moteId);
	}

	public MsgUint32 getMoteId(int index)
	{
		return (MsgUint32) moteIds.get(index);
	}
}
