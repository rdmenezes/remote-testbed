package diku.distlab.motecontrolclientlib.client;

import java.util.Observable;
import java.util.Observer;

public class MoteControlStatus {

	public static final short CONTROL_UNINITIALIZED = 0;
	public static final short CONTROL_GRANTED = 1;
	public static final short CONTROL_REVOKED = 2;
	public static final short CONTROL_RELEASED = 3;

	protected short status = CONTROL_UNINITIALIZED;
	protected Mote mote;
	protected ExposedObservable eventDispatcher = new ExposedObservable();

	protected class ObserverWrapper implements Observer
	{
		protected MoteControlStatusListener listener;
		protected Mote mote;

		public ObserverWrapper(Mote mote, MoteControlStatusListener listener)
		{
			super();
			this.listener = listener;
			this.mote = mote;
		}

		public void update(Observable o, Object arg) {
			this.listener.moteControlStatusChange(this.mote);
		}
	}

	protected MoteControlStatus(Mote mote)
	{
		this.mote = mote;
	}

	public boolean isControlled() {
		return status == CONTROL_GRANTED;
	}

	public short get()
	{
		return status;
	}

	protected void grantControl()
	{
		if (this.status != CONTROL_GRANTED)
		{
			this.status = CONTROL_GRANTED;
			this.fireChangeEvent();
		}
	}

	protected void releaseControl()
	{
		if (this.status == CONTROL_GRANTED)
		{
			this.status = CONTROL_RELEASED;
			this.fireChangeEvent();
		}
	}

	protected void revokeControl()
	{
		if (this.status == CONTROL_GRANTED)
		{
			this.status = CONTROL_REVOKED;
			this.fireChangeEvent();
		}
	}

	public void addChangeListener(MoteControlStatusListener listener)
	{
		eventDispatcher.addWrappedObserver(listener,new ObserverWrapper(this.mote,listener));
	}

	public void removeChangeListener(MoteControlStatusListener listener)
	{
		eventDispatcher.deleteWrappedObserver(listener);
	}

	protected void fireChangeEvent()
	{
		this.eventDispatcher.setChanged();
		this.eventDispatcher.notifyObservers();
	}

}
