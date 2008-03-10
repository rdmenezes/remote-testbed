package remote.gui;

import remote.motecontrol.client.LedsInputStream;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class MoteLedsPanel extends JPanel {

	private static final long serialVersionUID = 221423246112076656L;
	private JLabel led0,led1,led2,led3;
	private ImageIcon gray,red,yellow,green,blue;

	MoteLedsPanel()
	{
		java.net.URL grayURL = ReMote.class.getResource("/icons/led-gray.gif");
    	gray = new ImageIcon(grayURL);

    	java.net.URL redURL = ReMote.class.getResource("/icons/led-red.gif");
    	red = new ImageIcon(redURL);

    	java.net.URL greenURL = ReMote.class.getResource("/icons/led-green.gif");
    	green = new ImageIcon(greenURL);

    	java.net.URL yellowURL = ReMote.class.getResource("/icons/led-yellow.gif");
    	yellow = new ImageIcon(yellowURL);

    	java.net.URL blueURL = ReMote.class.getResource("/icons/led-blue.gif");
    	blue = new ImageIcon(blueURL);

    	led0 = new JLabel(gray);
    	led1 = new JLabel(gray);
    	led2 = new JLabel(gray);
    	led3 = new JLabel(gray);

    	add(led0);
    	add(led1);
    	add(led2);
    	add(led3);
	}

	public void clearLeds()
	{
		led0.setIcon(gray);
		led1.setIcon(gray);
		led2.setIcon(gray);
		led3.setIcon(gray);
	}

	public void setLeds(LedsInputStream leds)
	{
		if (leds.red())
		{
			led0.setIcon(red);
		}
		else
		{
			led0.setIcon(gray);
		}
		if (leds.green())
		{
			led1.setIcon(green);
		}
		else
		{
			led1.setIcon(gray);
		}
		if (leds.yellow())
		{
			led2.setIcon(yellow);
		}
		else
		{
			led2.setIcon(gray);
		}
		if (leds.blue())
		{
			led3.setIcon(blue);
		}
		else
		{
			led3.setIcon(gray);
		}
	}
}
