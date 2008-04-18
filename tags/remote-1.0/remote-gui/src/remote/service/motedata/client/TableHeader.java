package remote.service.motedata.client;

public interface TableHeader {

	public abstract Table getTable();

	public abstract int indexOf(String columnName) throws Exception;

	public abstract String getTitle(String columnName) throws Exception;

	public abstract String getTitle(int columnIndex);

	public abstract String getName(int columnIndex);

	public abstract Class getClass(int columnIndex)
			throws ClassNotFoundException;

	public abstract Class getClass(String columnName) throws Exception;

	public abstract boolean isVisible(String columnName) throws Exception;

	public abstract boolean isVisible(int columnIndex);

	public int countVisible();

}