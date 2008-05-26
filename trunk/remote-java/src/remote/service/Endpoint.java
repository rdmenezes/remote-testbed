package remote.service;

/** Endpoint information.
 *
 * Holds information about how to contact a testbed service,
 * such as hostname, port number of the mote control
 * host, and port and path information for web services.
 */
public class Endpoint {

	/** Hostname for connecting to the service. */
	String host;

	/** Port number for connecting to the service. */
	short port;

	/** Path for connecting to the service. */
	String path;

	//Endpoint(String host, String path, short port)
}
