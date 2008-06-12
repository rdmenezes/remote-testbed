package remote.service.authentication;

import remote.service.Service;
import remote.service.ServiceCallback;
import remote.session.Credential;

/** Service for session authenticator. */
public interface AuthenticationService extends Service {

	/** Get empty session credentials.
	 *
	 * The empty credentials can be used by a client to later
	 * authenticate the session. On success, the result object is
	 * an array of credential objects (Credential[]) defining
	 * the credentials needed for authentication.
	 *
	 * @param session	ID of the session to get empty credentials.
	 * @param callback	Callback for receiving result or error.
	 */
	void getEmptyCredentials(String session, ServiceCallback<Credential[]> callback);

	/** Authenticate session using with given credentials.
	 *
	 * Authenticate an already created session with set of credential
	 * provided by the client. On success, the result object is a
	 * Boolean object indicating whether the session was authenticated.
	 *
	 * @param session	ID of the session to authenticate.
	 * @param credentials	Client supplied credentials.
	 * @param callback	Callback for receiving result or error.
	 */
	void authenticate(String session, Credential[] credentials,
			  ServiceCallback<Boolean> callback);

}
