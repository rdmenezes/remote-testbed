package remote.gui.util;

import remote.motecontrol.client.SimpleMote;
import remote.motecontrol.client.SimpleMoteStatus;
import remote.protocol.motecontrol.MsgCommand;
import remote.protocol.motecontrol.MsgResult;
import remote.service.motedata.client.Table;
import remote.service.motedata.client.TableHeader;
import remote.service.motedata.client.TableRow;

public class MoteControlRow extends MoteControl implements TableRow, Comparable{

	public static final int COL_MOTE_ID = 0;

	public static final int COL_SITE = 1;

	public static final int COL_TOSADDRESS = 2;

	public static final int COL_MACADDRESS = 3;

	public static final int COL_STATUS = 4;

	public static final int COL_LAST_REQUEST = 5;

	public static final int COL_LEDS = 6;

	public static final int COL_LOGFILE = 7;

	MoteControlTable parentTable;

	TableRow infoRow;

	MoteControlRow(SimpleMote mote, boolean enableLeds, TableRow infoRow,
			MoteControlTable parentTable) {
		super(mote, enableLeds);
		this.parentTable = parentTable;
		this.infoRow = infoRow;
	}

	public Table getTable() {
		return parentTable;
	}

	public Object get(int columnIndex) {
		try {
			switch (columnIndex) {
			case COL_MOTE_ID:
				return this.infoRow.get("mote_id");
			case COL_SITE:
				return this.infoRow.get("site");
			case COL_TOSADDRESS:
				return this.infoRow.get("tosaddress");
			case COL_MACADDRESS:
				return this.infoRow.get("macaddress");
			case COL_STATUS:
				return MoteStatusRenderer.getText(this.getMote().getStatus().getStatus());
			case COL_LAST_REQUEST:
				return decodeLastCommandResult(this.getMote().getStatus());
			case COL_LEDS:
				return this.moteLeds;
			case COL_LOGFILE:
				if (this.logging) return this.logFile.getPath();
				else return "";
			}
		} catch (Exception e) {}
		return null;
	}

	public Object get(String columnName) throws Exception {
		return this.get(this.parentTable.getHeader().indexOf(columnName));
	}

	public int compareTo(Object o) {
		if (o==this) return 0;
		return 1;
	}

	public static String decodeLastCommandResult(SimpleMoteStatus s)
	{
		String prefix,suffix;
		switch(s.getLastCommand())
		{
			case -1:
				prefix = "No messages received yet - ";
				break;
			case MsgCommand.CANCELPROGRAMMING:
				prefix = "Cancel programming ";
				break;
			case MsgCommand.PROGRAM:
				prefix = "Programming ";
				break;
			case MsgCommand.RESET:
				prefix = "Reset ";
				break;
			case MsgCommand.START:
				prefix = "Start ";
				break;
			case MsgCommand.STATUS:
				prefix = "Status request ";
				break;
			case MsgCommand.STOP:
				prefix = "Stop ";
				break;
			default:
				prefix = "Unknown command ";
		}
		switch(s.getLastResult())
		{
			case -1:
				suffix = "no result available";
				break;
			case MsgResult.FAILURE:
				suffix = "failed";
				break;
			case MsgResult.NOT_SUPPORTED:
				suffix = "is not supported";
				break;
			case MsgResult.SUCCESS:
				suffix = "succesful";
				break;
			default:
				suffix = "unknown result";
		}
		return prefix+suffix;
	}

}
