package diku.distlab.motecontrolclientlib.client;

import diku.distlab.motecontrolclientlib.protocols.clientserver.*;
import diku.distlab.motecontrolclientlib.protocols.*;

import java.util.*;

/**
 * Created on Sep 19, 2005
 * @author zept
 */
public class Session {

	protected Thread serviceThread;
	protected Connection serverConnection;
	protected MoteManager moteManager;
	protected ExposedObservable eventDispatcher;
	protected String sessionId = null;

	protected class ObserverWrapper implements Observer
	{
		protected SessionEventListener listener;
		protected Session client;

		public ObserverWrapper(Session client, SessionEventListener listener)
		{
			super();
			this.listener = listener;
			this.client = client;
		}

		public void update(Observable o, Object arg) {
			this.listener.eventFired(this.client,(SessionEvent)arg);
		}
	}

	public Session(MoteManager moteManager)
	{
		super();
		this.moteManager = moteManager;
		this.moteManager.session = this;
		eventDispatcher = new ExposedObservable();
	}

	public MoteManager getMoteManager() {
		return moteManager;
	}

	public boolean isConnected() {
		if (serverConnection != null ) return serverConnection.isOpen();
		else return false;
	}

	public void connect(ConnectionInfo connectionInfo) throws Exception
	{
		if (serverConnection != null && serverConnection.isOpen()) disconnect();

		serverConnection = new Connection(connectionInfo);

		fireSessionEvent(SessionEvent.CONNECTED);

		this.serviceThread();
	}

	public void disconnect()
	{
		try
		{
			if (serviceThread != null && serviceThread.isAlive())
			{
				serviceThread.interrupt();
			}
			if (serverConnection != null && serverConnection.isOpen())
			{
				serverConnection.close();
				fireSessionEvent(SessionEvent.DISCONNECTED);
			}
			this.moteManager.revokeAllMoteControls();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	protected void requestControl(long moteids[])
	{
		ClientMsg message = new ClientMsg();

		message.getType().setValue(ClientMsgType.CLIENTREQUEST);
		message.getClientRequest().getCommand().setValue(MsgClientCommand.MSGCLIENTCOMMAND_GETMOTECONTROL);

		MsgMoteIdList idlist = message.getClientRequest().getMoteIdList();

		for (int i = 0; i < moteids.length; i++)
		{
			idlist.addMoteId(new MsgUint32(moteids[i]));
		}
		try
		{
			sendMsg(message);
		}
		catch (Exception e)
		{
			disconnect();
		}
	}

	protected void releaseControl(long[] moteids)
	{
		ClientMsg message = new ClientMsg();

		message.getType().setValue(ClientMsgType.CLIENTREQUEST);
		message.getClientRequest().getCommand().setValue(MsgClientCommand.MSGCLIENTCOMMAND_DROPMOTECONTROL);

		MsgMoteIdList idlist = message.getClientRequest().getMoteIdList();

		for (int i = 0; i < moteids.length; i++)
		{
			idlist.addMoteId(new MsgUint32(moteids[i]));
		}
		try
		{
			sendMsg(message);
		}
		catch (Exception e)
		{
			disconnect();
		}
	}

	protected void sendMoteMsg(MsgIO moteMsg, long[] moteids)
	{
		ClientMsg message = new ClientMsg();
		message.getType().setValue(ClientMsgType.CLIENTREQUEST);
		message.getClientRequest().getCommand().setValue(MsgClientCommand.MSGCLIENTCOMMAND_MOTEMESSAGE);
		try {
			message.getClientRequest().getMoteMsg().setData(moteMsg.getBytes());
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		MsgMoteIdList idlist = message.getClientRequest().getMoteIdList();
		for (int i = 0; i < moteids.length; i++)
		{
			idlist.addMoteId(new MsgUint32(moteids[i]));
		}
		try{
			sendMsg(message);
		}
		catch (Exception e)
		{
			disconnect();
		}
	}

	public void serviceThread() throws Exception
	{
		Runnable r = new Runnable() {
            public void run() {
            	ClientMsg message;
            	try
            	{
            		do
            		{
            			message = recvMsg();
            			if (message.getType().getValue() == ClientMsgType.SESSION)
            			{
            				//FIXME: signed/unsigned problem for large session ids
            				sessionId = Long.toString(message.getSession().getSessionId().getValue());
            				fireSessionEvent(SessionEvent.AUTHENTICATE);
            			}
            			else if ( message.getType().getValue() != ClientMsgType.CLIENTCONFIRM)
            			{
            				throw new Exception("Got unknown message from server!");
            			} else if (message.getClientConfirm().getResult().getValue() != MsgResult.SUCCESS)
            			{
            				if (message.getClientConfirm().getCommand().getValue() != MsgClientCommand.MSGCLIENTCOMMAND_GETMOTECONTROL)
            				{
            					moteManager.revokeMoteControl(message.getClientConfirm().getMote_id().getValue());
            				}
            				else
            				{
            					System.out.println("Control of mote ID "+message.getClientConfirm().getMote_id().getValue()+" was not granted by control server.");
            					// FIXME: notify that mote control was not granted
            				}
            			}
            			else
            			{
	            			switch (message.getClientConfirm().getCommand().getValue())
	            			{
	            				case MsgClientCommand.MSGCLIENTCOMMAND_GETMOTECONTROL:
	            					moteManager.grantMoteControl(message.getClientConfirm().getMote_id().getValue());
	            					break;
	            				case MsgClientCommand.MSGCLIENTCOMMAND_DROPMOTECONTROL:
	            					moteManager.revokeMoteControl(message.getClientConfirm().getMote_id().getValue());
	            					break;
	            				case MsgClientCommand.MSGCLIENTCOMMAND_MOTEMESSAGE:
	            					moteManager.confirm(message.getClientConfirm().getMote_id().getValue(),
	            							            message.getClientConfirm().getMoteMsg());
	            					break;
	            				default:
	            					throw new Exception("Got unknown message from server!");
	            			}
            			}
            		} while(true);
            	}
            	catch (Exception e)
				{
            		e.printStackTrace();
            		disconnect();
				}
            }
		};

        serviceThread = new Thread(r);
        serviceThread.start();
	}

	public void addSessionListener(SessionEventListener clientListener) {
		eventDispatcher.addWrappedObserver(clientListener,new ObserverWrapper(this,clientListener));
	}

	public void removeSessionListener(SessionEventListener clientListener)
	{
		eventDispatcher.deleteWrappedObserver(clientListener);
	}

	protected void fireSessionEvent(short eventId)
	{
		this.eventDispatcher.setChanged();
		this.eventDispatcher.notifyObservers(new SessionEvent(this,eventId));
	}

	protected ClientMsg recvMsg() throws Exception
	{
		MsgPayload message = new MsgPayload();
		message.read(serverConnection.is);
		ClientMsg msg = new ClientMsg();
		msg.setBytes(message.getData());
		return msg;
	}

	protected void sendMsg(ClientMsg msg) throws Exception
	{
		MsgPayload message = new MsgPayload();
		message.setData(msg.getBytes());
		message.write(serverConnection.os);
	}

	public String getSessionId() {
		return sessionId;
	}

	public Connection getServerConnection() {
		return serverConnection;
	}
}
