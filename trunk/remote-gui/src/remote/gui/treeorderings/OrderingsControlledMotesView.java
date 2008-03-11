package remote.gui.treeorderings;

import remote.gui.IconResources;
import remote.gui.RenderableTreeNode;

import java.awt.Color;
import java.awt.Component;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;

import remote.gui.util.MoteControlRow;
import remote.gui.util.TableRowOrdering;

import remote.motecontrol.client.SimpleMoteStatus;
import remote.protocol.motecontrol.MsgResult;
import remote.service.motedata.client.TableRow;

public class OrderingsControlledMotesView {

	private static class MoteControlNodeRenderer extends RenderableTreeNode {

		private JPanel panel = new JPanel();
		private JLabel staticLabel = new JLabel();
		private JLabel statusLabel = new JLabel();
		private JLabel errorLabel = new JLabel();

		private MoteControlRow row = null;

		public MoteControlNodeRenderer(Comparable key, MoteControlRow row, boolean numeric) {
			super(key, numeric);
			this.row = row;
			panel.setBackground(new Color(0, true));

			staticLabel.setIcon(IconResources.MOTE);
			errorLabel.setIcon(IconResources.ERROR);
			errorLabel.setVisible(false);

			try {
				staticLabel.setText(key.toString());
			} catch (Exception e) {
				e.printStackTrace();
			}

			staticLabel.setToolTipText(getToolTip(row));
			panel.add(staticLabel);
			panel.add(statusLabel);
			panel.add(errorLabel);
			this.refresh();
		}

		private void updateDynamicLabel() {
			ImageIcon icon = null;
			if (row == null) return;
			try {
				switch (row.getMote().getStatus().getStatus()) {
				case SimpleMoteStatus.UNAVAILABLE:
					break;
				case SimpleMoteStatus.REQ_CANCELPROGRAMMING:
					icon = IconResources.CANCEL_FLASH_MOTE;
					break;
				case SimpleMoteStatus.REQ_PROGRAM:
				case SimpleMoteStatus.PROGRAMMING:
					icon = IconResources.FLASH_MOTE;
					break;
				case SimpleMoteStatus.REQ_RESET:
					icon = IconResources.RESET_MOTE;
					break;
				case SimpleMoteStatus.REQ_START:
				case SimpleMoteStatus.REQ_STOP:
					break;
				case SimpleMoteStatus.RUNNING:
					icon = IconResources.START_MOTE;
					break;
				case SimpleMoteStatus.STOPPED:
					icon = IconResources.STOP_MOTE;
					break;
				case SimpleMoteStatus.UNKNOWN:
				default:
				}
				if (icon != null) {
					this.statusLabel.setIcon(icon);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}


		}

		public void refresh()
		{
			this.updateDynamicLabel();
			if (this.row.getMote().getStatus().getLastResult() != MsgResult.SUCCESS )
			{
				this.errorLabel.setVisible(true);
				panel.setToolTipText(getToolTip(row)+" - "+MoteControlRow.decodeLastCommandResult(this.row.getMote().getStatus()));
			}
			else
			{
				this.errorLabel.setVisible(false);
				panel.setToolTipText(getToolTip(row)+" - "+row.get(MoteControlRow.COL_STATUS).toString());
			}
		}

		public Component getRenderable() {
			this.refresh();
			return panel;
		}

		protected static String getToolTip(TableRow row) {
			try {
				return row.get("tosaddress") + " - (" + row.get("macaddress")
						+ ") @ " + row.get("site");
			} catch (Exception e) {
				e.printStackTrace();
				return "";
			}
		}

	};

	private static class RenderableOrdering extends SimpleTableRowOrdering {
		boolean numeric;

		public RenderableOrdering(String key, String name, boolean total, boolean numeric)
		{
			super(key,name,total);
			this.numeric = numeric;
		}

		public Comparable getKey(TableRow row) {
			return new MoteControlNodeRenderer(super.getKey(row), (MoteControlRow) row, numeric);
		}
	};

	public static TableRowOrdering BY_STATUS = new SimpleTableRowOrdering("status","status",false);

	public static TableRowOrdering BY_MAC = new RenderableOrdering("macaddress", "MAC", true, false);

	public static TableRowOrdering BY_TOS = new RenderableOrdering("tosaddress", "TOS", true, true);

	public static TableRowOrdering[] orderings = { BY_TOS, BY_MAC,BY_STATUS };

}
