package diku.distlab.remote.client.motedata;

import diku.distlab.remote.motedata.ColumnHeader;

public class ColumnHeaderWrapper implements TableHeader {

	ColumnHeader[] columnHeaders;
	Table parentTable;
	int visibleCount;

	protected ColumnHeaderWrapper(Table parentTable,ColumnHeader[] columnHeaders) {
		super();
		this.columnHeaders = columnHeaders;
		this.parentTable = parentTable;
		this.initVisibleCount();
	}

	public int indexOf(String columnName) throws Exception
	{
		for (int i = 0; i < columnHeaders.length; i++ )
		{
			if (columnName.equals(columnHeaders[i].getName()))
			{
				return i;
			}
		}
		throw new Exception("Column name not found!");
	}

	public String getTitle(String columnName) throws Exception {
		return this.getTitle(this.indexOf(columnName));
	}

	public String getTitle(int columnIndex) {
		return this.columnHeaders[columnIndex].getTitle();
	}

	public String getName(int columnIndex) {
		return this.columnHeaders[columnIndex].getName();
	}

	public Class getClass(int columnIndex) throws ClassNotFoundException {
		return Class.forName(columnHeaders[columnIndex].getValueclass());
	}

	public Class getClass(String columnName) throws Exception {
		return this.getClass(this.indexOf(columnName));
	}

	public Table getTable() {
		return this.parentTable;
	}

	public boolean isVisible(String columnName) throws Exception {
		return this.isVisible(this.indexOf(columnName));
	}

	public boolean isVisible(int columnIndex) {
		return this.columnHeaders[columnIndex].isVisible();
	}

	public int countVisible()
	{
		return visibleCount;
	}

	private void initVisibleCount()
	{
		this.visibleCount = 0;
		for ( int i = 0; i < this.columnHeaders.length; i++ )
		{
			if (this.columnHeaders[i].isVisible())
			{
				this.visibleCount++;
			}
		}
	}
}
