package remote.service.authentication.client;

import java.rmi.RemoteException;

import diku.distlab.remote.authentication.*;

public class ClientAuthenticator implements Authenticator {

	AuthenticatorServiceLocator authLocator;

	public ClientAuthenticator(String server)
	{
		authLocator = new AuthenticatorServiceLocator();
		String url ="http://"+server+":8080/axis/services/authentication";
		authLocator.setauthenticationEndpointAddress(url);

	}

	public Credential[] getEmptyCredentials() throws RemoteException {
		Credential[] creds = null;
		try
		{
			creds = authLocator.getauthentication().getEmptyCredentials();
		} catch (javax.xml.rpc.ServiceException e)
		{
			e.printStackTrace();
		}
		return creds;
	}

	public boolean authenticate(String session_id, Credential[] credentials) throws RemoteException {
		boolean result = false;
		try
		{
			result = authLocator.getauthentication().authenticate(session_id,credentials);
		} catch (javax.xml.rpc.ServiceException e)
		{
			e.printStackTrace();
		}
		return result;
	}

}
