/** Service for acquiring mote access and control privilege.
 *
 * This service provides clients with an interface for taking
 * control over motes. The set of motes the client wants to get
 * access to can either be chosen by a user if the client runs in
 * interactive mode, or can be preconfigured if the client runs in
 * automated mode. For each mote that are queried using
 * getMoteControlPrivileges(), an associated boolean value is
 * returned indicating whether the privileges were granted for the
 * mote or denied.
 */

package remote.service.moteaccess;