/*
 * Created on Sep 28, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.motecontrolclientlib.client;

import java.util.Observable;
import java.util.Hashtable;
import java.util.Collection;
import java.util.Collections;
import java.util.Observer;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MoteList {

	private Hashtable motes;
	protected ExposedObservable eventDispatcher = new ExposedObservable();

	protected class MoteListEvent
	{
		public static final byte ADDED = 0;
		public static final byte REMOVED = 1;
		public static final byte CLEARED = 2;

		public byte value;
		public Mote mote;

		MoteListEvent(byte value, Mote mote)
		{
			this.value = value;
			this.mote = mote;
		}

	}

	protected class ObserverWrapper implements Observer
	{
		protected MoteListListener listener;
		protected MoteList moteList;

		public ObserverWrapper(MoteList moteList, MoteListListener listener)
		{
			super();
			this.listener = listener;
			this.moteList = moteList;
		}

		public void update(Observable o, Object arg) {
			MoteListEvent event = (MoteListEvent)arg;
			switch (event.value)
			{
				case MoteListEvent.ADDED:
					this.listener.addedMote(moteList,event.mote);
					break;
				case MoteListEvent.REMOVED:
					this.listener.removedMote(moteList,event.mote);
					break;
				case MoteListEvent.CLEARED:
					this.listener.cleared(moteList);
					break;
			}

		}
	}

	protected MoteList()
	{
		motes = new Hashtable();
	}

	protected void add( Mote mote )
	{
		Long keyobj = new Long(mote.getId());
		motes.put(keyobj,mote);
		fireEvent(MoteListEvent.ADDED,mote);
	}

	protected Mote remove(long key)
	{
		Long keyobj = new Long(key);
		Mote mote = (Mote)motes.remove(keyobj);
		if (mote!=null)
		{
			fireEvent(MoteListEvent.REMOVED,mote);
		}
		return mote;
	}

	protected void clear()
	{
		motes.clear();
		fireEvent(MoteListEvent.CLEARED,null);
	}

	protected void fireEvent(byte eventValue,Mote mote)
	{
		eventDispatcher.setChanged();
		eventDispatcher.notifyObservers(new MoteListEvent(eventValue,mote));
	}

	public Mote get(long key)
	{
		return (Mote)motes.get(new Long(key));
	}

	public int length()
	{
		return motes.size();
	}

	public Collection getCollectionView()
	{
		return Collections.unmodifiableCollection(motes.values());
	}

	protected Collection getCollection()
	{
		return motes.values();
	}

	public void addListener(MoteListListener listener)
	{
		eventDispatcher.addWrappedObserver(listener,new ObserverWrapper(this,listener));
	}

	public void removeListener(MoteListListener listener)
	{
		eventDispatcher.deleteWrappedObserver(listener);
	}

}
