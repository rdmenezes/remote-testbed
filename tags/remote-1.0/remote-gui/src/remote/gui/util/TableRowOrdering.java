package remote.gui.util;
import remote.service.motedata.client.TableRow;

public interface TableRowOrdering {

	public abstract Comparable getKey(TableRow row);
	public abstract boolean isTotal();
	public abstract String toString();
}
