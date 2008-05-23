package remote.session;

/**
 *
 * @author fonseca
 */
public interface SessionManagerListener {
	void connected(Session session);
	void disconnected(Session session);
	void authenticated(Session session);
}
