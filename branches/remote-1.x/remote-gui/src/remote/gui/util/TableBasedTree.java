package remote.gui.util;

import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeModelEvent;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.ListIterator;
import remote.service.motedata.client.Table;
import remote.service.motedata.client.TableListener;
import remote.service.motedata.client.TableRow;

public class TableBasedTree implements TreeModel, TableListener {

	private Table table;
	private TableRowOrdering[] orderings;
	//private int [] grouping;
	private SortedTreeNode root;
	private Object rootValue;
	private HashSet listeners;
	private Hashtable rows;

	public TableBasedTree(Table table, Object rootValue, TableRowOrdering[] orderings) throws Exception
	{
		super();
		this.table = table;
		this.table.addListener(this);
		this.rootValue = rootValue;
		this.listeners = new HashSet();
		this.rows = new Hashtable();
		this.setOrderings(orderings);
	}

	public void setOrderings(TableRowOrdering[] orderings) throws Exception
	{
		this.orderings = orderings;

		this.updateTable();
	}

	public Object getRoot() {
		return root;
	}

	public Object getChild(Object parent, int index) {
		return ((SortedTreeNode)parent).getChild(index);
	}

	public int getChildCount(Object parent) {
		return ((SortedTreeNode)parent).getChildCount();
	}

	public boolean isLeaf(Object node) {
		return ((SortedTreeNode)node).isLeaf();
	}

	public void valueForPathChanged(TreePath path, Object newValue) {
		// TODO Auto-generated method stub

	}

	public int getIndexOfChild(Object parent, Object child) {
		return ((SortedTreeNode)parent).indexOf((SortedTreeNode)child);
	}

	public void addTreeModelListener(TreeModelListener l) {
		listeners.add(l);
	}

	public void removeTreeModelListener(TreeModelListener l) {
		listeners.remove(l);

	}

	public void updatedTable(Table table) {
		updateTable();
	}

	public void addedRow(Table table, TableRow row)
	{
		this.addTableRow(row,true);
	}

	public void deletedRow(Table table, TableRow row) {
		this.deleteTableRow(row, true);
	}

	public void updatedCell(Table table, TableRow row, int colIndex) {
		// first, get the old path for the row
		SortedTreeNode oldNode = (SortedTreeNode)rows.get(row);
		ArrayList oldPath = oldNode.getPath();
		ListIterator oldPathIter = oldPath.listIterator();
		// then, try reinserting the row, checking correspondance with the old path
		int j;
		boolean newPath = false;
		SortedTreeNode parent,node;
		Object value;
		parent = this.root;
		ArrayList path = new ArrayList();
		path.add(parent);
		oldPathIter.next(); // just skip the root
		for ( j = 0; j < this.orderings.length; j++ )
		{

			value = this.orderings[j].getKey(row);
			node = parent.getChild(value);

			if (node == null)
			{
				node = new SortedTreeNode(value);
				parent.addChild(node);
				this.notifyInsert(path.toArray(),new Object[]{node});
			}

			if (!newPath )
			{
				newPath = node != (SortedTreeNode)oldPathIter.next();
			}
			parent = node;
			path.add(parent);
		}

		if (newPath) // remove the old path nodes
		{
			this.deleteTableRow(row,true);
			parent.addChild(oldNode);
			this.notifyInsert(path.toArray(),new Object[]{oldNode});
			rows.put(row,oldNode);
		}
		else
		{
			this.notifyUpdate(oldNode);
			this.notifyUpdate(oldNode.getParent());
		}

	}

	private void updateTable()
	{
		this.root = new SortedTreeNode(this.rootValue);

		for ( int i = 0; i < table.rows(); i++ )
		{
			addTableRow(table.getRow(i),false);
		}
		this.notifyTreeStructureChanged();
	}

	private void addTableRow(TableRow row, boolean notify)
	{
		int j;
		SortedTreeNode parent,node;
		Object value;
		parent = this.root;
		java.util.Vector path = new java.util.Vector();
		path.add(parent);
		for ( j = 0; j < this.orderings.length; j++ )
		{
			value = this.orderings[j].getKey(row);
			node = parent.getChild(value);
			if (node == null)
			{
				node = new SortedTreeNode(value);
				parent.addChild(node);
				if (notify)
				{
					this.notifyInsert(path.toArray(),new Object[]{node});
				}
			}
			parent = node;
			path.add(parent);
		}
		node = new SortedTreeNode(row);
		rows.put(row,node);
		parent.addChild(node);
		if (notify)
		{
			this.notifyInsert(path.toArray(),new Object[]{node});
		}
	}

	private void deleteTableRow(TableRow row, boolean notify )
	{
		SortedTreeNode node = (SortedTreeNode)rows.remove(row);
		ArrayList path = node.getPath();

		if (node != null) // the node was found, now delete the node
		{
			int i = path.size()-1;
			SortedTreeNode parent = node.getParent();
			while(node.isLeaf() && parent != null)
			{
				int [] indices = {parent.indexOf(node)};

				parent.deleteChild(node.getValue());

				if (notify )
				{
					this.notifyRemove(path.subList(0,i).toArray(),indices,new Object[]{node});
				}

				node = parent;
				parent = node.getParent();
				i--;
			}
		}
	}

	private void notifyTreeStructureChanged()
	{
		TreeModelListener l;
		Object[] path = {this.root};
		TreeModelEvent e = new TreeModelEvent(this,path);
		Iterator it = listeners.iterator();
		while(it.hasNext())
		{
			l = (TreeModelListener)it.next();
			l.treeStructureChanged(e);
		}
	}

	private void notifyUpdate(SortedTreeNode node)
	{
		TreeModelListener l;
		TreeModelEvent e = new TreeModelEvent(this,node.getPath().toArray(),new int[]{node.getParent().indexOf(node)},new Object[]{node});
		Iterator it = listeners.iterator();
		while(it.hasNext())
		{
			l = (TreeModelListener)it.next();
			l.treeNodesChanged(e);
		}
	}

	private void notifyInsert(Object[] path, Object[] children)
	{
		TreeModelListener l;
		int[] indices = new int[children.length];
		for (int i=0;i<indices.length;i++)
		{
			indices[i] = ((SortedTreeNode)path[path.length-1]).indexOf((SortedTreeNode)children[i]);
		}
		TreeModelEvent e = new TreeModelEvent(this,path,indices,children);
		Iterator it = listeners.iterator();
		while(it.hasNext())
		{
			l = (TreeModelListener)it.next();
			l.treeNodesInserted(e);
		}
	}

	private void notifyRemove(Object[] path, int[] indices, Object[] children)
	{
		TreeModelListener l;
		TreeModelEvent e = new TreeModelEvent(this,path,indices,children);
		Iterator it = listeners.iterator();
		while(it.hasNext())
		{
			l = (TreeModelListener)it.next();
			l.treeNodesRemoved(e);
		}
	}
}
