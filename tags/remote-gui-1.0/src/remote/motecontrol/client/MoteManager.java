package remote.motecontrol.client;

import remote.protocol.MsgPayload;

public abstract class MoteManager {

	protected Session session;
	protected MoteList moteList;

	protected MoteManager()
	{
		this.moteList = new MoteList();
	}

	protected abstract void grantMoteControl(long mote_id);
	protected abstract void revokeMoteControl(long mote_id);
	protected abstract void revokeAllMoteControls();
	protected abstract void confirm(long mote_id, MsgPayload moteMsg);

	public abstract void requestMotes(long[] moteids);
	public abstract void releaseMotes(long[] moteids);
	public abstract void releaseAllMotes();

	public MoteList getMoteList()
	{
		return moteList;
	}
}
