package remote.gui;

import remote.motecontrol.client.LedsInputStream;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class MoteLedsPanel extends JPanel
{
	
	private static final long serialVersionUID = 221423246112076656L;
	private JLabel led0,led1,led2,led3;

	MoteLedsPanel()
	{
		led0 = new JLabel(IconResources.LED_GRAY);
		led1 = new JLabel(IconResources.LED_GRAY);
		led2 = new JLabel(IconResources.LED_GRAY);
		led3 = new JLabel(IconResources.LED_GRAY);
		
		add(led0);
		add(led1);
		add(led2);
		add(led3);
	}

	public void clearLeds()
	{
		led0.setIcon(IconResources.LED_GRAY);
		led1.setIcon(IconResources.LED_GRAY);
		led2.setIcon(IconResources.LED_GRAY);
		led3.setIcon(IconResources.LED_GRAY);
	}

	public void setLeds(LedsInputStream leds)
	{
		if (leds.red())
		{
			led0.setIcon(IconResources.LED_RED);
		}
		else
		{
			led0.setIcon(IconResources.LED_GRAY);
		}
		if (leds.green())
		{
			led1.setIcon(IconResources.LED_GREEN);
		}
		else
		{
			led1.setIcon(IconResources.LED_GRAY);
		}
		if (leds.yellow())
		{
			led2.setIcon(IconResources.LED_YELLOW);
		}
		else
		{
			led2.setIcon(IconResources.LED_GRAY);
		}
		if (leds.blue())
		{
			led3.setIcon(IconResources.LED_BLUE);
		}
		else
		{
			led3.setIcon(IconResources.LED_GRAY);
		}
	}
}
