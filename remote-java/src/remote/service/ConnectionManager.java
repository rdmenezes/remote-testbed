package remote.service;

/** Connection manager.
 *
 * The connection manager provides an abstraction for connecting
 * to the different services provided by a testbed. It keeps track
 * of what methods of transport, protocols and RPC the testbed
 * supports and acts as an intermediate layer between service
 * providers (the actual connectors to services) and session
 * management.
 */
public interface ConnectionManager {

}
