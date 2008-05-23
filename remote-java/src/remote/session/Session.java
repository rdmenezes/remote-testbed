package remote.session;

import remote.control.MoteManager;

/** Session information.
 *
 * Container for managing information about a session.
 */
public class Session {

	/** The ID of the session.
	 *
	 * Used when contacting session services. Corresponds to
	 * the session record in the testbed database.
	 */
	String id;

	/** User/project credentials.
	 *
	 * The credentials used when creating the session.
	 */
	Credential[] credentials;

	/** Connection information.
	 *
	 * Information about how to connect and make requests
	 * to the testbed of the session.
	 */
	Connection connection;

	/** The mote manager.
	 *
	 * Manages information about all motes in the testbed the session
	 * is connected to, such as controlled, reserved, and unavailable
	 * motes.
	 */
	MoteManager moteManager;

	String getId()
	{
		return id;
	}

	MoteManager getMoteManager(Session session)
	{
		return moteManager;
	}

	Credential[] getCredentials()
	{
		return credentials;
	}

	Connection getConnection()
	{
		return connection;
	}

}
