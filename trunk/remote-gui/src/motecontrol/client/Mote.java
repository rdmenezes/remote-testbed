/*
 * Created on Sep 18, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

package diku.distlab.motecontrolclientlib.client;
/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Mote
{
	protected long id;
	protected MoteControlStatus controlStatus;
	protected Mote(long id) {
		super();
		this.id = id;
		controlStatus = new MoteControlStatus(this);
	}

	public long getId() {
		return id;
	}

	public MoteControlStatus getControlStatus() {
		return controlStatus;
	}


}
