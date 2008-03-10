package remote.gui;

import javax.swing.ImageIcon;

public class IconResources {

	public static ImageIcon MOTE;
	public static ImageIcon STOP_MOTE;
	public static ImageIcon START_MOTE;
	public static ImageIcon FLASH_MOTE;
	public static ImageIcon CANCEL_FLASH_MOTE;
	public static ImageIcon RESET_MOTE;
	public static ImageIcon MOTE_CONSOLE;
	public static ImageIcon MOTE_RELEASE;
	public static ImageIcon CONSOLE_CLEAR;
	public static ImageIcon VIEW_REFRESH;
	public static ImageIcon ERROR;
	public static ImageIcon NEW_LOG;
	public static ImageIcon DELETE_LOG;
	public static ImageIcon START_LOG;
	public static ImageIcon STOP_LOG;
	public static ImageIcon SAVE;

	public static void initialize()
	{
		MOTE = new ImageIcon(IconResources.class.getResource("/icons/mote16x16.png"));
		STOP_MOTE = new ImageIcon(IconResources.class.getResource("/icons/stop16.gif"));
		START_MOTE = new ImageIcon(IconResources.class.getResource("/icons/play16.gif"));
		FLASH_MOTE = new ImageIcon(IconResources.class.getResource("/icons/lightning16.gif"));
		CANCEL_FLASH_MOTE = new ImageIcon(IconResources.class.getResource("/icons/blast16.gif"));
		RESET_MOTE = new ImageIcon(IconResources.class.getResource("/icons/redo16.gif"));
		MOTE_CONSOLE = new ImageIcon(IconResources.class.getResource("/toolbarButtonGraphics/development/Host16.gif"));
		MOTE_RELEASE = new ImageIcon(IconResources.class.getResource("/toolbarButtonGraphics/general/Delete16.gif"));
		CONSOLE_CLEAR = new ImageIcon(IconResources.class.getResource("/icons/delete16.gif"));
		VIEW_REFRESH = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Refresh16.gif"));
		ERROR = new ImageIcon(ReMote.class.getResource("/icons/error.png"));
		NEW_LOG = new ImageIcon(ReMote.class.getResource("/icons/new16.gif"));
		DELETE_LOG = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Delete16.gif"));
		START_LOG = new ImageIcon(ReMote.class.getResource("/icons/import16.gif"));
		STOP_LOG = new ImageIcon(ReMote.class.getResource("/icons/close16.gif"));
		SAVE = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Save16.gif"));
	}
}
