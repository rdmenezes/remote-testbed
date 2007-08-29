package gui;

import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JTree;
import javax.swing.JTable;
import javax.swing.UIManager;

import java.awt.Color;
import java.awt.Component;
import javax.swing.table.TableColumn;

import util.SortedTreeNode;

import diku.distlab.remote.client.motedata.Table;
import diku.distlab.remote.client.motedata.TableHeader;
import diku.distlab.remote.client.motedata.TableRow;

public class TableBasedTreeCellRenderer extends DefaultTreeCellRenderer {

	private static final long serialVersionUID = -2523748028729445113L;

	public Component getTreeCellRendererComponent(JTree tree, Object value,
			boolean sel, boolean expanded, boolean leaf, int row,
			boolean hasFocus) {

		if (value.getClass() == SortedTreeNode.class)
		{
			value = ((SortedTreeNode)value).getValue();
		}

		if (RenderableTreeNode.class.isAssignableFrom(value.getClass()))
		{
			RenderableTreeNode node = (RenderableTreeNode)value;
			JComponent c = (JComponent)node.getRenderable();

			if (sel)
			{
				c.setBorder(BorderFactory.createLineBorder(UIManager.getColor("desktop")));
				c.setBackground(UIManager.getColor("textHighlight"));
			}
			else
			{
				c.setBackground(new Color(0,true));
				c.setBorder(BorderFactory.createEmptyBorder());
			}
			return c;
		}
		else if (TableRow.class.isAssignableFrom(value.getClass()))
		{
			TableRow tableRow = (TableRow)value;
			Table table = tableRow.getTable();
			TableHeader header = table.getHeader();
			String[] headers = {"",""};
			Object[][] data= new Object[header.countVisible()][2];
			int j = 0;
			for ( int i=0; i < table.columns(); i++ )
			{
				if (header.isVisible(i))
				{
					data[j][0] = header.getTitle(i);
					data[j][1] = tableRow.get(i);
					j++;
				}
			}
			JTable jTable = new JTable(data,headers);
			jTable.setShowGrid(false);
			// FIXME: calculate dimensions
			jTable.setPreferredSize(new java.awt.Dimension(300,170));
			return jTable;
		}

		super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);

		return this;
	}
}
