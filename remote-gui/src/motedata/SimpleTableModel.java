package diku.distlab.remote.client.motedata;

import java.util.Vector;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;
import diku.distlab.remote.motedata.*;

public class SimpleTableModel extends AbstractTableModel implements TableModel {

	private static final long serialVersionUID = -1512789246313566828L;
	public SimpleTable baseTable;
	private Vector columnRefs;

	public SimpleTableModel(SimpleTable table) {
		super();
		setTable(table);
	}

	public void setTable(SimpleTable table)
	{
		baseTable = table;

		columnRefs = new Vector();

		for (int i = 0 ; i < baseTable.getColumnHeaders().length; i++)
		{
			if (baseTable.getColumnHeaders()[i].isVisible())
			{
				columnRefs.add(new Integer(i));
			}
		}
		this.fireTableDataChanged();
	}

	public Class getColumnClass(int columnIndex)
	{
		Class c;
		try
		{
			int index = ((Integer)columnRefs.get(columnIndex)).intValue();
			c = Class.forName(baseTable.getColumnHeaders()[index].getValueclass());
		}
		catch (Exception e)
		{
			c = String.class;
		}
		return c;
	}

	public int getColumnCount()
	{
		return columnRefs.size();
	}

	public int getRowCount()
	{
		int len = baseTable.getData().length;
		if (len == 0) return 1;
		return len;
	}

	public Object getValueAt(int rowIndex, int columnIndex)
	{
		if (baseTable.getData().length == 0)
		{
			return null;
		}
		// small trick to expose the index in the baseTable when using sorters
		if (columnIndex == -1)
		{
			return new Integer(rowIndex);
		}
		else
		{
			int col = ((Integer)columnRefs.get(columnIndex)).intValue();
			return baseTable.getData()[rowIndex][col];
		}
	}

	public String getColumnName(int columnIndex)
	{
		int index = ((Integer)columnRefs.get(columnIndex)).intValue();
		return baseTable.getColumnHeaders()[index].getTitle();
	}
}
