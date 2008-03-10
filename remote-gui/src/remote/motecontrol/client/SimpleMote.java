package remote.motecontrol.client;

import java.io.*;
import remote.protocol.MsgPayload;

public class SimpleMote extends Mote {

	protected SimpleMoteStatus status;
	protected SimpleMoteManager moteManager;
	protected PipedOutputStream dataOutput;
	protected Thread dataThread;

	protected SimpleMote(long id,SimpleMoteManager moteManager) {
		super(id);
		this.moteManager = moteManager;
		this.status = new SimpleMoteStatus(this);
	}

	public SimpleMoteStatus getStatus() {
		return status;
	}

	protected void dataOut(MsgPayload data)
	{
		if (dataOutput != null)
		{
			try
			{
				byte[] buf = data.getData();
				dataOutput.write(buf);
				dataOutput.flush();
			}
			catch (Exception e)
			{
				//e.printStackTrace();
				try
				{
					dataOutput.close();
				}
				catch (IOException e2){	 e2.printStackTrace();}
				finally
				{
					dataOutput = null;
				}

			}
		}

	}

	public void connectDataSource(final InputStream dataInput)
	{
		// create a thread reading from the stream and creating data output
		Runnable r = new Runnable() {
            public void run() {
            	BufferedInputStream input = new BufferedInputStream(dataInput);
            	byte[] bytes;
            	long skipped;
            	try{
            		while(true)
            		{
            			input.mark(200);
            			skipped = input.skip(199);
            			bytes = new byte[(int)skipped];
            			input.reset();
            			input.read(bytes);
            			moteManager.sendData(id,bytes);
            		}
            	}
            	catch (Exception e)
            	{
            		e.printStackTrace();
            	}
            }
		};

		dataThread = new Thread(r);
		dataThread.start();
	}

	public void connectDataSource(final PipedOutputStream source)
	{
		// create a thread reading from the stream and creating data output
		Runnable r = new Runnable() {
            public void run() {
            	try{
            		PipedInputStream dataInput = new PipedInputStream(source);
            		BufferedInputStream input = new BufferedInputStream(dataInput);
            		byte[] bytes;
                	long skipped;
            		while(true)
            		{
            			input.mark(200);
            			skipped = input.skip(199);
            			bytes = new byte[(int)skipped];
            			input.reset();
            			input.read(bytes);
            			moteManager.sendData(id,bytes);
            		}
            	}
            	catch (Exception e)	{e.printStackTrace();}
            }
		};

		dataThread = new Thread(r);
		dataThread.start();
	}

	public void connectDataSink(PipedInputStream sink) throws IOException
	{
		try
		{
			if (this.dataOutput != null) this.dataOutput.close();
		}
		catch (Exception e)	{e.printStackTrace();}
		finally
		{
			this.dataOutput = new PipedOutputStream(sink);
		}
	}
}
