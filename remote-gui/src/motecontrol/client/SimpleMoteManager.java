package diku.distlab.motecontrolclientlib.client;
import java.util.ArrayList;

import diku.distlab.motecontrolclientlib.protocols.MsgPayload;
import diku.distlab.motecontrolclientlib.protocols.motecontrol.*;

public class SimpleMoteManager extends MoteManager {

	public SimpleMoteManager() {
		super();
	}

	protected void grantMoteControl(long mote_id) {
		SimpleMote mote;
		mote = (SimpleMote)moteList.get(mote_id);
		if (mote == null)
		{
			mote = new SimpleMote(mote_id,this);
			moteList.add(mote);
		}
		mote.getControlStatus().grantControl();
		// automatically detect new mote status
		status(toArray(mote.getId()));
	}

	protected void revokeMoteControl(long mote_id) {
		SimpleMote mote;
		mote = (SimpleMote) moteList.remove(mote_id);
		if (mote != null)
		{
			mote.getControlStatus().revokeControl();
			mote.getStatus().setStatus(SimpleMoteStatus.UNAVAILABLE);
		}

	}

	protected void revokeAllMoteControls()
	{
		Object[] motes = moteList.getCollection().toArray();

		for (int i=0;i<motes.length;i++)
		{
			revokeMoteControl(((Mote)motes[i]).getId());
		}
	}

	protected void confirm(long mote_id, MsgPayload moteMsg)
	{
		MoteMsg msg = new MoteMsg();
		try
		{
			msg.setBytes(moteMsg.getData());
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		SimpleMote mote;
		mote = (SimpleMote)moteList.get(mote_id);
		if (mote != null)
		{
			switch (msg.getType().getValue())
			{
				case MoteMsgType.CONFIRM:
					mote.getStatus().update(msg.getConfirm());
					break;
				case MoteMsgType.DATA:
					mote.dataOut(msg.getData());
					break;
				default:
					// TODO:take corrective action here
					//throw new Exception("Invalid mote message!");
			}
		}
	}

	public void requestMotes(long[] moteids)
	{
		session.requestControl(moteids);
	}

	public void releaseMotes(long[] moteids) {
		Mote mote;
		for (int i = 0; i < moteids.length; i++)
		{
			mote = moteList.remove(moteids[i]);
			if (mote != null) mote.getControlStatus().releaseControl();
		}
		session.releaseControl(moteids);
	}

	public void releaseAllMotes()
	{
		// TODO: release all mote controls
	}

	public void cancelProgramming(long[] moteids) {
		requestCommand(MsgCommand.CANCELPROGRAMMING, moteids);
	}

	public void status(long[] moteids) {
		requestCommand(MsgCommand.STATUS, moteids);
	}

	public void start(long[] moteids) {
		requestCommand(MsgCommand.START, moteids);
	}

	public void stop(long[] moteids) {
		requestCommand(MsgCommand.STOP, moteids);
	}

	public void reset(long[] moteids) {
		requestCommand(MsgCommand.RESET, moteids);
	}

	public void program(long[] moteids, byte[] flashImage) {
		MoteMsg msg = new MoteMsg();
		msg.getType().setValue(MoteMsgType.REQUEST);
		msg.getRequest().getCommand().setValue(MsgCommand.PROGRAM);
		msg.getRequest().getFlashImage().setData(flashImage);
		session.sendMoteMsg(msg,filterReady(moteids,msg));
	}

	protected void sendData(long moteid, byte[] data) {
		MoteMsg msg = new MoteMsg();
		msg.getType().setValue(MoteMsgType.DATA);
		msg.getData().setData(data);
		session.sendMoteMsg(msg,toArray(moteid));
	}

	private long[] filterReady(long[] moteids,MoteMsg msg)
	{
		SimpleMote mote;
		int i;
		ArrayList list = new ArrayList();
		for (i=0; i < moteids.length; i++)
		{
			mote = (SimpleMote)moteList.get(moteids[i]);
			if (mote !=null && mote.getStatus().isReady())
			{
				list.add(new Long(moteids[i]));
				mote.getStatus().update(msg.getRequest());
			}
		}
		long[] res = new long[list.size()];
		for (i=0; i < list.size(); i++)
		{
			res[i] = ((Long)list.get(i)).longValue();
		}
		return res;
	}

	private void requestCommand(short command,long[] moteids)
	{
		MoteMsg msg = new MoteMsg();
		msg.getType().setValue(MoteMsgType.REQUEST);
		msg.getRequest().getCommand().setValue(command);
		session.sendMoteMsg(msg,filterReady(moteids,msg));
	}

	private long[] toArray(long value)
	{
		long[] res = new long[1];
		res[0] = value;
		return res;
	}
}
