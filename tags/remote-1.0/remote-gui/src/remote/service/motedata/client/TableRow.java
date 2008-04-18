package remote.service.motedata.client;

public interface TableRow {

	public abstract Table getTable();

	public abstract Object get(int columnIndex);

	public abstract Object get(String columnName) throws Exception;

}