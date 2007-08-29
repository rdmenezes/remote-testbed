/*
 * Created on Sep 30, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.client;
import java.io.InputStream;
import java.io.FilterInputStream;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class LedsInputStream extends FilterInputStream {

	protected class ObserverWrapper implements Observer
	{
		protected LedsListener listener;
		protected LedsInputStream leds;

		public ObserverWrapper(LedsInputStream leds, LedsListener listener)
		{
			super();
			this.listener = listener;
			this.leds = leds;
		}

		public void update(Observable o, Object arg) {
			this.listener.ledsChanged(this.leds);
		}
	}

	protected ExposedObservable eventDispatcher = new ExposedObservable();

	public LedsInputStream(InputStream in) {
		super(in);
	}

	public void setInputStream(InputStream in)
	{
		super.in = in;
	}

	protected int state,newstate;

	protected byte[] filter( byte bytes[] )
	{
		byte[] temp = new byte[bytes.length];
		int j=0,i=0;
		for (i=0; i < bytes.length; i++)
		{
			if ( bytes[i]>>4 == 1 )
			{
				this.setState(bytes[i] & 0x0F);
			}
			else
			{
				temp[j++] += bytes[i];
			}
		}
		byte[] res =new byte[j];
		for (i=0;i<j;i++)
		{
			res[i] = temp[i];
		}
		return res;
	}

	protected int filter(int b)
	{
		if (b>=0)
		{
			if ( b>>4 == 1 )
			{
				this.newstate = b & 0x0F;
				return -1;
			}
		}
		return b;
	}

	protected void setState(int state)
	{
		if (this.state != state)
		{
			this.state = state;
			this.eventDispatcher.setChanged();
			this.eventDispatcher.notifyObservers();
		}
	}

	public int read() throws IOException
	{
		int b;
		while(true)
		{
			b = in.read();
			if ( b < 0 ) return b;
			if ( filter(b) < 0 )
			{
				this.setState(this.newstate);
			}
			else
			{
				return b;
			}
		}
	}

	public int read(byte[] b, int off, int len) throws IOException
	{
		int read = 0;
		int v;
		while (read < len)
		{
			v = in.read();
			if (v>=0)
			{
				if (this.filter(v)>=0)
				{
					b[off++] = (byte)v;
					read++;
				}
			}
			else
			{
				return v;
			}
		}
		this.setState(this.newstate);
		return read;
	}

	public void addListener(LedsListener listener)
	{
		eventDispatcher.addWrappedObserver(listener,new ObserverWrapper(this,listener));
	}

	public void removeListener(LedsListener listener)
	{
		eventDispatcher.deleteWrappedObserver(listener);
	}

	public boolean red()
	{
		return ((state & 0x01) == 0x01);
	}

	public boolean green()
	{
		return ((state & 0x02) == 0x02);
	}

	public boolean yellow()
	{
		return ((state & 0x04) == 0x04);
	}

	public boolean blue()
	{
		return ((state & 0x08) == 0x08);
	}
}
