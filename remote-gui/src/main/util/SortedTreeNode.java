package util;
import java.util.ArrayList;
import java.util.Collections;
import java.util.TreeMap;
import java.util.SortedMap;

public class SortedTreeNode {
	private SortedMap children;
	private Object value;
	private SortedTreeNode parent;

	public SortedTreeNode(Object value)
	{
		this.value = value;
		this.parent = null;
		this.children = new TreeMap();
	}

	public int getChildCount()
	{
		return children.size();
	}

	public boolean isLeaf()
	{
		return children.size() == 0;
	}

	public void addChild(SortedTreeNode childNode)
	{
		childNode.parent = this;
		children.put(childNode.value,childNode);
	}

	public void deleteChild(Object key)
	{
		SortedTreeNode child = (SortedTreeNode)children.remove(key);
		if (child != null)
		{
			child.parent = null;
		}
	}

	public SortedTreeNode getChild(int index)
	{
		return (SortedTreeNode)children.values().toArray()[index];
	}

	public SortedTreeNode getChild(Object key)
	{
		return (SortedTreeNode)children.get(key);
	}

	public Object getValue() {
		return value;
	}

	public void setValue(Object value) {
		this.value = value;
	}

	public int indexOf(SortedTreeNode child)
	{
		Object[] c = children.values().toArray();
		for (int i=0; i < c.length;i++)
		{
			if (child == c[i]) return i;
		}
		return -1;
	}

	public ArrayList getPath()
	{
		ArrayList path = new ArrayList();
		path.add(this);
		SortedTreeNode parent = this.parent;
		while( parent != null )
		{
			path.add(parent);
			parent = parent.parent;
		}
		Collections.reverse(path);
		return path;
	}

	public SortedTreeNode getParent()
	{
		return parent;
	}
}