package remote.service;

/** Connection information.
 *
 * Holds information about a testbed connection and how to use the services
 * it provides, such as hostname, port number of the mote control
 * host, and port and path information for web services.
 */
public class Connection {

	/** Hostname for connecting to the service. */
	String host;

	/** Port number for connecting to the service. */
	short port;

	/** Path for connecting to the service. */
	String path;

	//Connection(String host, String path, short port)
}
