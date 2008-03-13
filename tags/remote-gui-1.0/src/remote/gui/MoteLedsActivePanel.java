/*
 * Created on Sep 30, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package remote.gui;

import remote.motecontrol.client.LedsInputStream;
import remote.motecontrol.client.LedsListener;
import remote.motecontrol.client.SimpleMote;
import remote.motecontrol.client.SimpleMoteStatus;
import remote.motecontrol.client.SimpleMoteStatusListener;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MoteLedsActivePanel extends MoteLedsPanel implements LedsListener, SimpleMoteStatusListener {

	private static final long serialVersionUID = -5827572059829759381L;


	public MoteLedsActivePanel(LedsInputStream moteLeds, SimpleMoteStatus status)
	{
		moteLeds.addListener(this);
		status.addChangeListener(this);

	}

	public void ledsChanged(LedsInputStream leds) {
		this.setLeds(leds);
	}

	public void simpleMoteStatusChange(SimpleMote mote) {
		this.clearLeds();
	}

}
