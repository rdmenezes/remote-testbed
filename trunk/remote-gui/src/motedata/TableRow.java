package diku.distlab.remote.client.motedata;

public interface TableRow {

	public abstract Table getTable();

	public abstract Object get(int columnIndex);

	public abstract Object get(String columnName) throws Exception;

}