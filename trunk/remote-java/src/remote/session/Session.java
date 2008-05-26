package remote.session;

import remote.service.ConnectionManager;
import remote.control.MoteManager;

/** Session information.
 *
 * Container for managing information about a session.
 */
public class Session {

	/** The ID of the session. */
	String id;

	/** User/project credentials. */
	Credential[] credentials;

	/** Connection manager. */
	ConnectionManager connectionManager;

	/** The mote manager. */
	MoteManager moteManager;

	/** Whether or not the session has been authenticated. */
	boolean authenticated;

	/** Get session ID.
	 *
	 * The session ID can be used when contacting session services.
	 * Corresponds to the session record in the testbed database.
	 *
	 * @return	The session ID.
	 */
	public String getId()
	{
		return id;
	}

	/** Is the session authenticated?
	 *
	 * @return	True if the session has been authenticated.
	 */
	public boolean isAuthenticated()
	{
		return authenticated;
	}

	/** Get the mote manager for this session.
	 *
	 * Manages information about all motes in the testbed the session
	 * is connected to, such as controlled, reserved, and unavailable
	 * motes.
	 *
	 * @return	The mote manager.
	 */
	public MoteManager getMoteManager()
	{
		return moteManager;
	}

	/** Get session credentials.
	 *
	 * The credentials used when authenticating the session. This
	 * may be empty or invalid when the session has not yet been
	 * authenticated.
	 *
	 * @return	The session credentials.
	 */
	public Credential[] getCredentials()
	{
		return credentials;
	}

	/** Get connection information.
	 *
	 * Information about how to connect and make requests
	 * to the testbed of the session.
	 *
	 * @return	Information about how to connect.
	 */
	public ConnectionManager getConnectionManager()
	{
		return connectionManager;
	}

}
