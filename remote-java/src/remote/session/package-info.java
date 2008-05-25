/** Provides session management.
 *
 * <p> This package provides session management that clients can use to
 * create, connect, and authenticate new sessions, as well as destroying
 * a session. All sessions are managed by the session manager, which
 * provides a high-level interface independent of whatever underlying
 * service provider is used. By listening to session events clients can
 * interoperate with created sessions and show connection progress and
 * request input when a session needs to be authenticated. </p>
 *
 * <p><b>Session Creation and Authentication</b></p>
 *
 * <p> The client should perform the following steps in order to create and
 * authenticate a session: </p>
 *
 * <ul>
 *
 * <li> Create a session by providing connection information and a
 *	session event listener. </li>
 *
 * <li> When the session manager has connected it will signal the
 *	connected event in the listener interface. </li>
 *
 * <li> When the session is ready to be authenticated the session
 *	manager will signal the authenticate event in the listener
 *	interface. The empty credentials are then available in the
 *	session instance and can be used by the client to request input
 *	from the user. </li>
 *
 * <li> When the client has the credentials from the user it should
 *	request the session manager to authenticate the session using
 *	the authenticate method. </li>
 *
 * <li> Depending on whether the session authentication is successful or
 *	not the session manager will notify the client via the listener
 *	interface by either calling the authenticated method or calling
 *	the authenticate method. </li>
 *
 * </ul>
 *
 * <p><b>Notes</b></p>
 *
 * <p> During session creation and authentication only parts of the mote
 * manager may be functional. </p>
 *
 * <p> The session permits multiple variations of the number of credential
 * fields clients should provide and clients should support this
 * gracefully. It can choose not support all possible combinations.
 * Clients should assume that credentials that have been marked hidden
 * may contain information sensitive to the user and they should process
 * these credentials accordingly. </p>
 */

package remote.session;
