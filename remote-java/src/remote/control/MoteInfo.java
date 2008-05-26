package remote.control;

/** Basic mote information interface.
 *
 * This interface exposes all read-only attributes.
 */
public interface MoteInfo {

	/** Mac address attribute name. */
	public static final String MACADDRESS = "macaddress";

	/** Net address attribute name. */
	public static final String NETADDRESS = "netaddress";

	/** Platform attribute name. */
	public static final String PLATFORM = "platform";

	/** Get mote ID.
	 *
	 * @return	The mote ID.
	 */
	long getId();

	/** Get mote attribute.
	 *
	 * @param name	The attribute name.
	 * @return	The attribute value.
	 */
	String getAttribute(String name);

	/** Is the mote available for reservation.
	 *
	 * @return	True if the mote is not in use.
	 */
	boolean isAvailable();

	/** Is the mote controlled.
	 *
	 * @return	True if the mote is controlled by this session.
	 */
	boolean isControlled();

	/** Get location ID.
	 *
	 * @return	The location ID.
	 */
	long getLocationId();

	/** Get name of the mote location.
	 *
	 * @return	The mote location name.
	 */
	String getLocationName();

	/** Get mote host IP address.
	 *
	 * @return	The mote host IP address.
	 */
	String getHostIp();

	/** Get mote host DNS name.
	 *
	 * @return	The mote host DNS name.
	 */
	String getHostDns();

}
