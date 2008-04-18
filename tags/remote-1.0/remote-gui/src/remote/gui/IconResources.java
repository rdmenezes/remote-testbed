package remote.gui;

import javax.swing.ImageIcon;

public class IconResources {

	public static ImageIcon MOTE;
	public static ImageIcon MOTE_CONTROLLED;
	public static ImageIcon MOTE_OCCUPIED;
	public static ImageIcon STOP_MOTE;
	public static ImageIcon START_MOTE;
	public static ImageIcon FLASH_MOTE;
	public static ImageIcon CANCEL_FLASH_MOTE;
	public static ImageIcon RESET_MOTE;
	public static ImageIcon MOTE_CONSOLE;
	public static ImageIcon MOTE_RELEASE;
	public static ImageIcon MOTE_DIG528_2;
	public static ImageIcon MOTE_MICAZ;
	public static ImageIcon MOTE_TMOTE_SKY;
	public static ImageIcon CONSOLE_CLEAR;
	public static ImageIcon VIEW_REFRESH;
	public static ImageIcon ERROR;
	public static ImageIcon NEW_LOG;
	public static ImageIcon DELETE_LOG;
	public static ImageIcon START_LOG;
	public static ImageIcon STOP_LOG;
	public static ImageIcon SAVE;
	public static ImageIcon LED_BLUE;
	public static ImageIcon LED_GRAY;
	public static ImageIcon LED_GREEN;
	public static ImageIcon LED_RED;
	public static ImageIcon LED_YELLOW;

	public static void initialize()
	{
		MOTE = new ImageIcon(IconResources.class.getResource("/icons/mote.png"));
		MOTE_CONTROLLED = new ImageIcon(IconResources.class.getResource("/icons/user.gif"));
		MOTE_OCCUPIED = new ImageIcon(IconResources.class.getResource("/icons/lock_on.png"));

		STOP_MOTE = new ImageIcon(IconResources.class.getResource("/icons/stop16.gif"));
		START_MOTE = new ImageIcon(IconResources.class.getResource("/icons/play16.gif"));
		FLASH_MOTE = new ImageIcon(IconResources.class.getResource("/icons/lightning16.gif"));
		CANCEL_FLASH_MOTE = new ImageIcon(IconResources.class.getResource("/icons/blast16.gif"));
		RESET_MOTE = new ImageIcon(IconResources.class.getResource("/icons/redo16.gif"));
		MOTE_CONSOLE = new ImageIcon(IconResources.class.getResource("/toolbarButtonGraphics/development/Host16.gif"));
		MOTE_RELEASE = new ImageIcon(IconResources.class.getResource("/toolbarButtonGraphics/general/Delete16.gif"));
		MOTE_DIG528_2 = new ImageIcon(IconResources.class.getResource("/icons/mote-dig528-2.png"));
		MOTE_MICAZ = new ImageIcon(IconResources.class.getResource("/icons/mote-micaz.png"));
		MOTE_TMOTE_SKY = new ImageIcon(IconResources.class.getResource("/icons/mote-tmote-sky.png"));
		CONSOLE_CLEAR = new ImageIcon(IconResources.class.getResource("/icons/delete16.gif"));
		VIEW_REFRESH = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Refresh16.gif"));
		ERROR = new ImageIcon(ReMote.class.getResource("/icons/error.png"));
		NEW_LOG = new ImageIcon(ReMote.class.getResource("/icons/new16.gif"));
		DELETE_LOG = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Delete16.gif"));
		START_LOG = new ImageIcon(ReMote.class.getResource("/icons/import16.gif"));
		STOP_LOG = new ImageIcon(ReMote.class.getResource("/icons/close16.gif"));
		SAVE = new ImageIcon(ReMote.class.getResource("/toolbarButtonGraphics/general/Save16.gif"));

		LED_GRAY = new ImageIcon(IconResources.class.getResource("/icons/led-gray.gif"));
		LED_GREEN = new ImageIcon(IconResources.class.getResource("/icons/led-green.gif"));
		LED_RED = new ImageIcon(IconResources.class.getResource("/icons/led-red.gif"));
		LED_YELLOW = new ImageIcon(IconResources.class.getResource("/icons/led-yellow.gif"));
		LED_BLUE = new ImageIcon(IconResources.class.getResource("/icons/led-blue.gif"));
	}
}
