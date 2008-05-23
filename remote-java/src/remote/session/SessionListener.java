package remote.session;

/**
 *
 */
public interface SessionListener {

	/** Connected event.
	 *
	 * Signals that the session has been successfully connected
	 * to the testbed.
	 *
	 * @param session	The connected session.
	 */
	void connected(Session session);

	/** Authenticate event.
	 *
	 * Called when the client should authenticate the session, either
	 * after the session has been connected and empty credentials
	 * for the session are available, or after an attempt to authenticate
	 * has failed. Successful authentication of the session will be
	 * signaled using the SessionListener.authenticated event.
	 *
	 * @param session	The session that needs authenticatioin.
	 */
	void authenticate(Session session);

	/** Authenticated event.
	 *
	 * Signals that the session has been successfully authenticated.
	 *
	 * @param session	The authenticated session.
	 */
	void authenticated(Session session);

	/** Disconnected event.
	 * 
	 * Signals that the connection of the session has been lost.
	 * After this event mote of the session will no longer be
	 * available.
	 *
	 * @param session	The disconnected session.
	 */
	void disconnected(Session session);

}
