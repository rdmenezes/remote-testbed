package diku.distlab.remote.client.motedata;

import diku.distlab.remote.motedata.*;
import java.util.HashSet;
import java.util.Iterator;

public class SimpleTableWrapper implements Table {
	SimpleTable simpleTable;
	TableHeader header;
	HashSet listeners;

	public SimpleTableWrapper(SimpleTable simpleTable) {
		super();
		this.simpleTable = simpleTable;
		this.header = new ColumnHeaderWrapper(this,simpleTable.getColumnHeaders());
		this.listeners = new HashSet();
	}

	protected void updateTable(SimpleTable simpleTable)
	{
		this.simpleTable = simpleTable;
		TableListener l;
		Iterator i = listeners.iterator();
		while(i.hasNext())
		{
			l = (TableListener)i.next();
			l.updatedTable(this);
		}
	}

	public TableHeader getHeader() {
		return header;
	}

	public TableRow getRow(int rowIndex)
	{
		return new SimpleTableRow(this,simpleTable.getData()[rowIndex]);
	}

	public int rows() {
		return simpleTable.getData().length;
	}

	public int columns() {
		return simpleTable.getColumnHeaders().length;
	}

	/** A very naive lookup implementation - but tables will be rather small, anyway **/
	public TableRow lookup(int columnIndex, Object value) {

		Object[][] values = simpleTable.getData();
		for (int i = 0; i < values.length; i++)
		{
			if (value.equals(values[i][columnIndex])) return new SimpleTableRow(this,values[i]);
		}
		return null;
	}

	public void addListener(TableListener l) {
		listeners.add(l);
	}

	public void removeListener(TableListener l) {
		listeners.remove(l);
	}
}
