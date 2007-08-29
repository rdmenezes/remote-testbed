package authentication;

import util.SQLHelper;

/**
 * @author Esben Zeuthen
 * This is an abstract class with the generic code for the session authentication
 * web service. To integrate authentication for a user account system, this class should be
 * inherited. The abstract methods should be implemented with the business logic
 * applicable to the user account system.
 */
public abstract class AbstractAuthenticator {

	/** This function should be implemented to return an array of empty
	 * credential fields applicable for the user account system used.
	 * @return Array of credential objects defining the credentials needed for authentication.
	 */
	abstract public Credential[] getEmptyCredentials() throws Exception;

	public boolean authenticate(String session_id, Credential[] credentials) throws Exception
	{
		boolean success = false;
		if (!checkCredentials(session_id,credentials)) return success;
		SQLHelper sql = null;
		Exception ex = null;
		try {
			String auth = "update session set auth=1 where id="+session_id;
			sql = new SQLHelper();
			sql.openDB();
			success = (sql.update(auth) == 1);
		} catch (Exception e) {
			ex = e;
		} finally {
			sql.closeDB();
			if (ex != null)
				throw ex;
		}
		return success;
	}

	/**
	 * This method should be implemented to check the credentials used for client authentication.
	 * This is the integration point for custom user account systems. It is also one of the places
	 * to perform integration between the user account system and the mote reservation system.
	 * @param session_id ID of the session object - for use with custom session attributes
	 * @param credentials The credentials supplied by the client
	 * @return authentication success or failure.
	 */
	abstract protected boolean checkCredentials(String session_id,Credential[] credentials) throws Exception;
}
