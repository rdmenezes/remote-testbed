package remote.service;

/** Manager of all services. */
public interface ServiceManager {
	public final static String GWT_MOTEACCESS = "GWT/moteaccess";
	public final static String WSDL_MOTEACCESS = "WSDL/moteaccess";

	boolean hasServiceProvider(String name);
	Service getServiceProvider(String name);
}