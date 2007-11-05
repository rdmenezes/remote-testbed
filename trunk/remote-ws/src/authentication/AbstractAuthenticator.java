package authentication;

import util.SQLHelper;

/** Abstract base class for session authenticator.
 *
 * This is an abstract base class with the generic code for the session
 * authentication web service. To integrate authentication for a user
 * account system, this class should be inherited. The abstract methods
 * should be implemented with the business logic applicable to the user
 * account system.
 *
 * @author Esben Zeuthen
 */
public abstract class AbstractAuthenticator {

	private static final String AUTH_QUERY = "update session set auth=1 where id=?";

	/** Get an empty array of credential fields.
	 *
	 * This function should be implemented to return an array of
	 * empty credential fields applicable for the user account
	 * system used.
	 *
	 * @return Array of credential objects defining the credentials
	 *	   needed for authentication.
	 * @throws Exception
	 */
	abstract public Credential[] getEmptyCredentials() throws Exception;

	/** Authenticate session using with given credentials.
	 *
	 * Authenticate an already open session with set of credential
	 * provided by the client.
	 *
	 * @param session_id	ID of the session to authenticate.
	 * @param credentials	Client supplied credentials.
	 *
	 * @return Whether session could be authenticated.
	 * @throws Exception
	 */
	public boolean authenticate(String session_id, Credential[] credentials) throws Exception
	{
		if (!checkCredentials(session_id, credentials))
			return false;

		SQLHelper sql = null;
		Object[] params = { session_id };

		try {
			sql = new SQLHelper();
			sql.openDB();
			return sql.update(AUTH_QUERY, params) == 1;

		} finally {
			sql.closeDB();
		}
	}

	/** Check the credentials used for client authentication.
	 *
	 * This method should be implemented to check the credentials
	 * used for client authentication.
	 *
	 * This is the integration point for custom user account
	 * systems. It is also one of the places to perform integration
	 * between the user account system and the mote reservation
	 * system.
	 *
	 * @param session_id    ID of the session object; for use with
	 *                      custom session attributes
	 * @param credentials   The credentials supplied by the client
	 *
	 * @return authentication success or failure.
	 * @throws Exception
	 */
	abstract protected boolean checkCredentials(String session_id, Credential[] credentials) throws Exception;
}
