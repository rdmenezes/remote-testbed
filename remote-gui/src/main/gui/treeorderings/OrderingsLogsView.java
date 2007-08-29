package gui.treeorderings;

import util.TableRowOrdering;

public class OrderingsLogsView {
	public static TableRowOrdering BY_NAME
	= new SimpleTableRowOrdering("name", "name", true);

	public static TableRowOrdering[] orderings = {BY_NAME};
}
