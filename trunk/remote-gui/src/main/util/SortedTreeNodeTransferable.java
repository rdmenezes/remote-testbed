package util;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;

public class SortedTreeNodeTransferable implements Transferable {

	private SortedTreeNode[] nodes;
	public static final String mimeType = DataFlavor.javaJVMLocalObjectMimeType + ";class=util.SortedTreeNode";
	private DataFlavor[] myFlavors;



	public SortedTreeNodeTransferable(SortedTreeNode[] nodes) {
		super();
		this.nodes = nodes;
		try
		{
			myFlavors = new DataFlavor[]{new DataFlavor(mimeType)};
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public DataFlavor[] getTransferDataFlavors() {
		return myFlavors;
	}

	public boolean isDataFlavorSupported(DataFlavor flavor) {
		return myFlavors[0].equals(flavor);
	}

	public Object getTransferData(DataFlavor flavor)
			throws UnsupportedFlavorException, IOException {
		if (!this.isDataFlavorSupported(flavor))
		{
			throw new UnsupportedFlavorException(flavor);
		}
		return nodes;
	}

}
