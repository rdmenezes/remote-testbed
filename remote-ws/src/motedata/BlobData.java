package motedata;

import java.io.Serializable;

public class BlobData implements Serializable
{

	/**
	 *
	 */
	private static final long serialVersionUID = -5958852541855343379L;

	private byte[] data;

	public BlobData()
	{
		data = null;
	}

	public BlobData(byte[] data) {
		super();
		// TODO Auto-generated constructor stub
		this.data = data;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}
}
