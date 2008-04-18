package remote.service.motedata.client;

public interface TableListener {
	public abstract void updatedTable( Table table );
	public abstract void addedRow( Table table, TableRow row );
	public abstract void deletedRow(Table table, TableRow row );
	public abstract void updatedCell(Table table, TableRow row, int colIndex);
}
