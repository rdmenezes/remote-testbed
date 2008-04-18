package remote.gui.util;

import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;
import java.awt.Component;
import java.util.Comparator;
import remote.motecontrol.client.SimpleMoteStatus;

public class MoteStatusRenderer extends JLabel implements TableCellRenderer, Comparator {

	/**
	 *
	 */
	private static final long serialVersionUID = 6986857411085856617L;

	public MoteStatusRenderer()
	{

	}

    public Component getTableCellRendererComponent(
            JTable table, Object value,
            boolean isSelected, boolean hasFocus,
            int row, int column) {
    	setText(getText(((SimpleMoteStatus)value).getStatus()));
    	return this;
    }

    public static String getText(short s)
	{
		switch(s)
		{
			case SimpleMoteStatus.UNAVAILABLE:
				return "Control unavailable!";
			case SimpleMoteStatus.REQ_CANCELPROGRAMMING:
				return "Cancelling programming...";
			case SimpleMoteStatus.REQ_PROGRAM:
			case SimpleMoteStatus.PROGRAMMING:
				return "Programming...";
			case SimpleMoteStatus.REQ_RESET:
				return "Resetting...";
			case SimpleMoteStatus.REQ_START:
				return "Starting...";
			case SimpleMoteStatus.REQ_STOP:
				return "Stopping..";
			case SimpleMoteStatus.RUNNING:
				return "Running";
			case SimpleMoteStatus.STOPPED:
				return "Stopped";
			case SimpleMoteStatus.UNKNOWN:
				return "Status unknown";
			default:
				return "Invalid status "+s;
		}
	}

	public int compare(Object arg0, Object arg1) {
		SimpleMoteStatus s0,s1;
		s0 = (SimpleMoteStatus)arg0;
		s1 = (SimpleMoteStatus)arg1;
		return getText(s0.getStatus()).compareTo(getText(s1.getStatus()));
	}
}
