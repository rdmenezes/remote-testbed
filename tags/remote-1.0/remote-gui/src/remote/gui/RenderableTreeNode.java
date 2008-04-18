package remote.gui;

import java.awt.Component;

public abstract class RenderableTreeNode implements Comparable
{

	private Comparable compareKey;
	private boolean numeric;
	private int intValue;

	protected RenderableTreeNode(Comparable compareKey, boolean numeric)
	{
		super();
		this.compareKey = compareKey;
		if (numeric && compareKey instanceof String) {
			this.intValue = Integer.parseInt((String) compareKey);
			this.numeric = numeric;
		}
	}

	public int compareTo(Object arg0)
	{
		if (!(arg0 instanceof RenderableTreeNode))
			return 0;

		RenderableTreeNode o = (RenderableTreeNode)arg0;
		if (this.numeric == true && o.numeric == true)
			return this.intValue - o.intValue;

		return this.compareKey.compareTo(o.compareKey);
	}

	public abstract Component getRenderable();
}
