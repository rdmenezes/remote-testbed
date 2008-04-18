/*
 * Created on Sep 18, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package remote.gui;
import java.util.Observer;
import java.util.Observable;
import javax.swing.*;
import remote.motecontrol.client.SimpleMote;
import remote.motecontrol.client.SimpleMoteStatus;
import remote.motecontrol.client.SimpleMoteStatusListener;

import remote.gui.util.MoteStatusRenderer;

/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MoteStatusIndicator extends JLabel implements SimpleMoteStatusListener {

	/**
	 *
	 */
	private static final long serialVersionUID = 340462691686092770L;
	SimpleMoteStatus status;

	public MoteStatusIndicator(SimpleMoteStatus p_status)
	{
		super("Status unknown");
		status = p_status;
		status.addChangeListener(this);
		setText(MoteStatusRenderer.getText(status.getStatus()));
	}

	public void update(Observable obs, Object obj)
	{
		setText(MoteStatusRenderer.getText(status.getStatus()));
	}

	public void simpleMoteStatusChange(SimpleMote mote) {
		setText(MoteStatusRenderer.getText(status.getStatus()));
	}



}
