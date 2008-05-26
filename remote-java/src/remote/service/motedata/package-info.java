/** Service for getting mote information.
 *
 * <p> This web service provides clients with a method to get status
 * and other information about motes in the testbed. Clients can
 * use this information to present the user with mote attributes
 * about mote availability, MAC address, etc. </p>
 *
 * <p> Clients are required to pass their session ID in order to allow
 * the web service to list client specific information, such as
 * mote availability and status, correctly. </p>
 *
 * <p> Clients should make as few assumptions about the result as
 * possible, since it can contain mote attributes unknown to the
 * client. It can use the various meta data information on each
 * attribute by inspecting the associated ColumnHeader.	</p>
 *
 * <p> Information about all motes is returned and there are no
 * possibilities for the client to set limits, for example on
 * the number of motes from which to have results. </p>
 */

package remote.service.motedata;