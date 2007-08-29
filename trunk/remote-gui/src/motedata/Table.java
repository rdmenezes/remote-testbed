package diku.distlab.remote.client.motedata;

public interface Table {

	public abstract TableHeader getHeader();

	public abstract TableRow getRow(int rowIndex);

	public abstract TableRow lookup(int columnIndex, Object value);

	public abstract int rows();

	public abstract int columns();

	public abstract void addListener(TableListener l);

	public abstract void removeListener(TableListener l);

}