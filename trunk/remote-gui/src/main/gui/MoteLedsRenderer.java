package gui;

import java.awt.Component;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;
import javax.swing.border.EmptyBorder;
import java.awt.Color;
import diku.distlab.motecontrolclientlib.client.LedsInputStream;


public class MoteLedsRenderer extends MoteLedsPanel implements TableCellRenderer
{
	private static final long serialVersionUID = -39829087289694471L;

	public MoteLedsRenderer()
	{
		this.setBorder(new EmptyBorder(-6,-6,-6,-6));
		this.setBackground(new Color(255,255,255));
	}

	public Component getTableCellRendererComponent(
            JTable table, Object value,
            boolean isSelected, boolean hasFocus,
            int row, int column) {
		setLeds(((LedsInputStream)value));
    	return this;
    }
}
