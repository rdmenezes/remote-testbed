package remote.session;

/** Session Manager.
 *
 * The session manager provides a factory-like interface for
 * creating and maintaining sessions.
 */
public interface SessionManager {

	/** Create a new session.
	 *
	 * Before the new session can be used it needs to first be connected
	 * and authenticated. Each of these steps steps will be signaled via
	 * the callbacks in the provided SessionListener.
	 *
	 * @param connection	Connection information.
	 * @param listener	The listener for session events.
	 * @return		The new session.
	 */
	Session create(Connection connection, SessionListener listener);

	/** Request authentication of the session.
	 *
	 * After creating a new session the session manager will try to connect
	 * to the testbed and get the empty credentials for the session.
	 * Once this has been signaled via the SessionListener.authenticate
	 * callback the client should fill in the empty credentials and
	 * request the session manager to authenticate the session.
	 *
	 * @param session
	 * @see SessionListener
	 */
	void authenticate(Session session);

	/** Destroy a session.
	 *
	 * Disconnect and destroy a session.
	 * 
	 * @param session	The session to destroy.
	 */
	void destroy(Session session);

}