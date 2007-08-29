package gui.treeorderings;

import util.TableRowOrdering;
import diku.distlab.remote.client.motedata.TableRow;

public class SimpleTableRowOrdering implements TableRowOrdering {

		String key,name;
		boolean total;

		public SimpleTableRowOrdering(String key, String name, boolean total)
		{
			this.key = key; this.name = name;; this.total = total;
		}

		public Comparable getKey(TableRow row) {
			Comparable value = null;
			try {
				value = (Comparable) row.get(key);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return value;
		}

		public boolean isTotal() {
			return total;
		}

		public String toString() {
			return name;
		}
}
