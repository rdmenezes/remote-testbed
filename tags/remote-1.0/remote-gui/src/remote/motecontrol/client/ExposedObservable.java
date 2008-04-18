package remote.motecontrol.client;

import java.util.*;

public class ExposedObservable extends Observable {

	protected Hashtable wrappedObservers = new Hashtable();

	protected void setChanged()
	{
		super.setChanged();
	}

	protected void addWrappedObserver(Object observer, Observer wrapper)
	{
		wrappedObservers.put(observer,wrapper);
		this.addObserver(wrapper);
	}

	protected void deleteWrappedObserver(Object observer)
	{
		this.deleteObserver((Observer)wrappedObservers.remove(observer));
	}

}
