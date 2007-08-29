package diku.distlab.remote.client.motedata;

public class SimpleTableRow implements TableRow {

	Table parentTable;
	Object[] entries;

	protected SimpleTableRow(Table parentTable, Object[] entries) {
		super();
		this.parentTable = parentTable;
		this.entries = entries;
	}

	public Object get(int columnIndex)
	{
		return this.entries[columnIndex];
	}

	public Object get(String columnName) throws Exception
	{
		return this.entries[parentTable.getHeader().indexOf(columnName)];
	}

	public Table getTable() {
		return this.parentTable;
	}
}
