package diku.distlab.remote.client.moteaccess;

import java.rmi.RemoteException;

import diku.distlab.remote.moteaccess.MoteAccess;
import diku.distlab.remote.moteaccess.MoteAccessServiceLocator;

public class ClientMoteAccess implements MoteAccess{

	MoteAccessServiceLocator masLocator;

	public ClientMoteAccess(String server)
	{
		masLocator = new MoteAccessServiceLocator();
		String url ="http://"+server+":8080/axis/services/moteaccess";
		masLocator.setmoteaccessEndpointAddress(url);

	}

	public boolean[] getMoteControlPrivileges(long[] mote_ids, String session_id) throws RemoteException {
		boolean[] results = null;
		try{
			results = masLocator.getmoteaccess().getMoteControlPrivileges(mote_ids,session_id);
		} catch (javax.xml.rpc.ServiceException e)
		{
			e.printStackTrace();
		}
		return results;
	}

}
