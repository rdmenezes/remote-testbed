package remote.gui.treeorderings;
import java.util.ArrayList;

import javax.swing.JComboBox;
import javax.swing.JList;
import javax.swing.plaf.basic.BasicComboBoxRenderer;

import remote.gui.util.TableBasedTree;
import remote.gui.util.TableRowOrdering;
import remote.gui.util.TreeOrderingHandler;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;



public class TreeGroupingSelector implements ActionListener{

	BasicComboBoxRenderer comboBoxRenderer = new BasicComboBoxRenderer()
	{
		public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus)
		{
			if (value != null && TableRowOrdering.class.isAssignableFrom(value.getClass()))
			{
				value = ((TableRowOrdering)value).toString();
			}
			return super.getListCellRendererComponent(list,value,index,isSelected,cellHasFocus);
		}
	};

	TableBasedTree tree;
	ArrayList orderings;
	final JComboBox selectors[];
	int currentLevel = 0;
	TreeOrderingHandler orderingHandler = new TreeOrderingHandler();

	public TreeGroupingSelector(TableBasedTree tree, TableRowOrdering[] orderings, JComboBox[] selectors) {
		super();
		// TODO Auto-generated constructor stub
		this.tree = tree;
		this.orderings = new ArrayList();
		for (int i = 0 ; i < orderings.length; i++)
		{
			this.orderings.add(i,orderings[i]);
		}
		this.selectors = selectors;

		for (int j = 0; j < this.selectors.length; j++)
		{
			this.selectors[j].addActionListener(this);
			this.selectors[j].setRenderer(comboBoxRenderer);
		}

		this.setComboBoxItems(0,orderings);
	}

	public void actionPerformed(ActionEvent e) {
		this.orderingHandler.clearCurrentOrderings();
		int i = 0;
		JComboBox source = (JComboBox)e.getSource();
		boolean passedSource = false;
		while (!passedSource && !this.orderingHandler.isUnique() && i < this.selectors.length)
		{
			TableRowOrdering ordering = (TableRowOrdering)this.selectors[i].getSelectedItem();
			if (ordering!=null)
			{
				this.orderingHandler.addOrdering(ordering);
			}
			passedSource = this.selectors[i]==source;
			i++;
		}


		if (this.orderingHandler.isUnique())
		{
			try
			{
				this.tree.setOrderings(this.orderingHandler.getOrderings());
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
			}

		}
		else if (i < this.selectors.length)
		{
			this.setComboBoxItems(i,this.orderingHandler.getAllowedOrderings((TableRowOrdering[])this.orderings.toArray(new TableRowOrdering[0])));
			i++;
		}

		for (int j = i; j < this.selectors.length; j++)
		{
			this.selectors[j].setVisible(false);
		}

	}


	private void setComboBoxItems(int i,TableRowOrdering[] orderings)
	{
		JComboBox box = selectors[i];
		box.removeAllItems();
		for (int j=0;j<orderings.length;j++)
		{
			box.addItem(orderings[j]);
		}
		box.setVisible(true);
	}
}
