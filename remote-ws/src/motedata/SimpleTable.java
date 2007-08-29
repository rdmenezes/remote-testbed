package motedata;

import java.io.Serializable;

public class SimpleTable implements Serializable {
	/**
	 *
	 */
	private static final long serialVersionUID = -6733140214602762727L;
	private ColumnHeader[] columnHeaders;
	private Object[][] data;

	public SimpleTable()
	{
		columnHeaders = null;
		data = null;
	}

	public SimpleTable(int rowCount, int colCount)
	{
		columnHeaders = new ColumnHeader[colCount];
		data = new Object[rowCount][colCount];
	}

	public ColumnHeader[] getColumnHeaders() {
		return columnHeaders;
	}

	public Object[][] getData() {
		return data;
	}

}
