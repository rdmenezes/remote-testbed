package remote.gui.treeorderings;

import java.awt.Color;
import java.awt.Component;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

import remote.gui.util.MoteControlRow;
import remote.gui.util.TableRowOrdering;
import remote.gui.ReMote;
import remote.gui.RenderableTreeNode;
import remote.gui.IconResources;

import remote.service.motedata.client.TableRow;

public class OrderingsAllMotesView {

	private static class MoteNodeRenderer extends RenderableTreeNode {
		JPanel mote = new JPanel();

		protected MoteNodeRenderer(Comparable key, TableRow row) {
			super(key);
			mote.setBackground(new Color(0, true));
			String labeltext = "";
			ImageIcon icon = IconResources.MOTE;
			JLabel label = new JLabel(icon);

			icon = null;
			labeltext = key.toString();
			try {
				String availability = row.get("mote_usage").toString();
				if (availability.equals("available")) {
					icon = null;
				} else if (availability.equals("occupied")) {
					icon = IconResources.MOTE_OCCUPIED;
				} else if (availability.equals("controlled")) {
					icon = IconResources.MOTE_CONTROLLED;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			label.setText(labeltext);
			mote.setToolTipText(getToolTip(row));
			mote.add(label);
			if (icon != null) {
				mote.add(new JLabel(icon));
			}
		}

		protected static String getToolTip(TableRow row) {
			try {
				return row.get("tosaddress")
				     + " - " + row.get("platform")
				     + " (" + row.get("macaddress") + ") @ "
				     + row.get("site");
			} catch (Exception e) {
				e.printStackTrace();
				return "";
			}
		}

		public Component getRenderable() {
			return mote;
		}

	}

	private static class RenderableOrdering extends SimpleTableRowOrdering {

		public RenderableOrdering(String key, String name, boolean total) {
			super(key, name, total);
		}

		public Comparable getKey(TableRow row) {
			return new MoteNodeRenderer(super.getKey(row), row);
		}
	};

	public static TableRowOrdering BY_USAGE
	= new SimpleTableRowOrdering("mote_usage", "Usage", false);/*

	public static TableRowOrdering BY_RESERVATION
	= new SimpleTableRowOrdering("reservation", "reservation", false);

	public static TableRowOrdering BY_ROOM
	= new SimpleTableRowOrdering("room", "room", false);**/

	public static TableRowOrdering BY_TOS
	= new RenderableOrdering("tosaddress","TOS",true);

	public static TableRowOrdering BY_MAC
	= new RenderableOrdering("macaddress","MAC",true);

	/**public static TableRowOrdering BY_MOTE_ID
	= new RenderableOrdering("mote_id","mote_id",true);**/


	public static TableRowOrdering[] orderings
	= { BY_TOS,BY_MAC,BY_USAGE };

}
