package remote.service;

/**
 *
 * @author fonseca
 */
public interface ServiceManager {
	public final static String GWT_MOTEACCESS = "GWT/moteaccess";
	public final static String WSDL_MOTEACCESS = "WSDL/moteaccess";

	boolean hasServiceProvider(String name);
	ServiceProvider getServiceProvider(String name);
}