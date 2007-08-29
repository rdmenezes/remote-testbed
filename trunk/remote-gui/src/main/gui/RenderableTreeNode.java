package gui;

import java.awt.Component;

public abstract class RenderableTreeNode implements Comparable{

	private Comparable compareKey;

	protected RenderableTreeNode(Comparable compareKey) {
		super();
		this.compareKey = compareKey;
	}

	public int compareTo(Object arg0) {
		RenderableTreeNode o = (RenderableTreeNode)arg0;
		return this.compareKey.compareTo(o.compareKey);
	}

	public abstract Component getRenderable();
}
