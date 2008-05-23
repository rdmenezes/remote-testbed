package remote.session;

import remote.control.MoteManager;

/**
 *
 * @author fonseca
 */
public interface SessionManager {
	Session create(/* ... */);
	void remove(Session session);
	void getEmptyCredentials();
	MoteManager getMoteManager(Session session);
}