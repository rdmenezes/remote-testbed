package remote.gui.treeorderings;

import remote.gui.util.TableRowOrdering;
import remote.service.motedata.client.TableRow;

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
