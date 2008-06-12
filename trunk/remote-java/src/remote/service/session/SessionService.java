package remote.service.session;

import remote.service.Service;
import remote.service.ServiceCallback;

/** Session life-cycle management.
 *
 * All interaction with a testbed is session-oriented. This
 * service provides a simple way for clients to manage the
 * life-cycle of a service.
 */
public interface SessionService extends Service {

	/** Create a new session.
	 *
	 * Requests an ID associated with a new and unauthenticated
	 * session. On success, the result object is a String
	 * containing the session ID.
	 *
	 * @param callback	Callback for receiving result or error.
	 */
	void create(ServiceCallback<String> callback);

	/** Destroy a session.
	 *
	 * On success, the result object is a String
	 * containing the ID of the destroyed session.
	 *
	 * @param session	ID of the session that should be destroyed.
	 * @param callback	Callback for receiving result or error.
	 */
	void destroy(String session, ServiceCallback<String> callback);

}
