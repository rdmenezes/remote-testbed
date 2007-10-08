package motedata;

import java.io.Serializable;

/** Simple data table.
 *
 * A simple container for storing data from a database query so it can
 * be exchanged between the server and client.
 */
public class SimpleTable implements Serializable {

	private static final long serialVersionUID = -6733140214602762727L;
	private ColumnHeader[] columnHeaders;
	private Object[][] data;

	/** Create a table with initial given size.
	 *
	 * @param rowCount	The number of rows in the table.
	 * @param colCount	The number of columns in the table.
	 */
	public SimpleTable(int rowCount, int colCount)
	{
		columnHeaders = new ColumnHeader[colCount];
		data = new Object[rowCount][colCount];
	}

	/** Dummy constructor to silence compiler warnings. */
	public SimpleTable()
	{
		this(0, 0);
	}

	/** Get table column headers.
	 *
	 * @return The column header of the table.
	 */
	public ColumnHeader[] getColumnHeaders()
	{
		return columnHeaders;
	}

	/** Get table data.
	 *
	 * @return The object matrix.
	 */
	public Object[][] getData() {
		return data;
	}

}
