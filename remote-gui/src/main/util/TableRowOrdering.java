package util;
import diku.distlab.remote.client.motedata.*;

public interface TableRowOrdering {

	public abstract Comparable getKey(TableRow row);
	public abstract boolean isTotal();
	public abstract String toString();
}
