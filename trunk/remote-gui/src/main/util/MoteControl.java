package util;
import diku.distlab.motecontrolclientlib.client.*;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PipedOutputStream;
import java.io.PipedInputStream;
import java.io.BufferedOutputStream;
import java.io.OutputStream;
import java.io.InputStream;

public class MoteControl {

	protected SimpleMote mote;
	protected LedsInputStream moteLeds;
	protected PipedOutputStream os;
	protected PipedInputStream dataSink = null;
	protected OutputStream sink;
	protected InputStream moteIn;
	protected boolean ledsEnabled = false;
	protected boolean logging = false;
	protected File logFile;
	protected Thread dataThread = null;

	MoteControl(SimpleMote mote, boolean enableLeds)
	{
		this.mote = mote;
		this.ledsEnabled = enableLeds;
		this.moteLeds =  new LedsInputStream(null);
		this.dataReader();

	}

	public OutputStream getOutputStream()
	{
		if (os == null)
		{
			os = new PipedOutputStream();
			mote.connectDataSource(os);
		}
		return os;
	}

	public void attachOutputStream(OutputStream sink)
	{
		this.sink = sink;
	}

	public void attachInputStream(InputStream source)
	{
		this.mote.connectDataSource(source);
	}

	protected void dataReader()
	{
		Runnable r = new Runnable() {

			private void configureStreams()
			{
				try{
					// First, create a dataSink pipe
					dataSink = new PipedInputStream();
					mote.connectDataSink(dataSink);
					moteIn = dataSink;

					// Then, attach a log stream if logging is enabled
					if (logging)
					{
						moteIn = new InputLogStream(new BufferedOutputStream(new FileOutputStream(logFile,true)),moteIn);
					}

					// Finally, attach the led stream if leds are enabled
					if (ledsEnabled)
					{
						moteLeds.setInputStream(moteIn);
						moteIn = moteLeds;
					}
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}

			}

            public void run() {
        		this.configureStreams();
   	    		int b = -1;
           		while(dataSink != null)
           		{
           			try
           			{
           				b = moteIn.read();
           			}
           			catch (IOException e)
           			{
           			//	e.printStackTrace();
           				break;
           			}

            		if (sink != null && b>=0)
            		{
            			try {
            				sink.write(b);
            			}
            			catch (IOException e)
            			{
            				e.printStackTrace();
            				sink = null;
            			}

            		}
            	}
           	}


		};

		if (dataThread != null && dataThread.isAlive())
		{
			dataThread.interrupt();
		}

		dataThread = new Thread(r);
		dataThread.start();
	}

	public void startLog(File file) throws Exception
	{
		if (!file.exists()) file.createNewFile();
		this.logFile = file;
		this.logging = true;
		dataReader();
	}

	public void stopLog()
	{
		if (!this.logging) return;
		this.logging = false;
		dataReader();
	}

	public SimpleMote getMote() {
		return mote;
	}

	public LedsInputStream getMoteLeds() {
		return moteLeds;
	}

	public boolean isLedsEnabled() {
		return ledsEnabled;
	}

	public void enableLeds() {
		if (this.ledsEnabled) return;
		this.ledsEnabled = true;
		dataReader();
	}

	public void disableLeds() {
		if (!this.ledsEnabled) return;
		this.ledsEnabled = false;
		dataReader();
	}

	public boolean isLogging() {
		return logging;
	}

}
