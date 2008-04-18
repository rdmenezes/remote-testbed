package remote.service.motedata.client;

import java.util.Hashtable;
import diku.distlab.remote.motedata.*;
import remote.motecontrol.client.Session;

public class MoteDataWrapper {
	private static MoteDataServiceLocator motedataLocator = new MoteDataServiceLocator();

	private static SimpleTable moteList = null;
	private static SimpleTableWrapper moteTable = null;
	private static Hashtable moteIndex = null;
	private static Session session = null;

	public static Table getMoteTable() {
		if (moteTable == null)
		{
			moteTable = new SimpleTableWrapper(getMoteList());
		}
		return moteTable;
	}

	public static Hashtable getMoteIndex() {
		return moteIndex;
	}

	public static void updateMoteList() {
		try
		{
			moteList = motedataLocator.getmotedata().getMoteData(session.getSessionId());
			((SimpleTableWrapper)getMoteTable()).updateTable(moteList);
			moteIndex = createIndex(moteList,0);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	private static SimpleTable getMoteList()
	{
		if (moteList == null)
		{
			try
			{
				moteList = motedataLocator.getmotedata().getMoteData(session.getSessionId());
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		return moteList;
	}

	static Hashtable createIndex(SimpleTable table, int col)
	{
		Hashtable index = new Hashtable();
		for (int i = 0 ; i < table.getData().length ; i ++ )
		{
			index.put(table.getData()[i][col], table.getData()[i]);
		}
		return index;
	}

	public static void setSession(Session session) {
		MoteDataWrapper.session = session;
		String url ="http://"+session.getServerConnection().getConnectionInfo().getServerName()+":8080/axis/services/motedata";
		motedataLocator.setmotedataEndpointAddress(url);
	}
}
