/** Service for client authentication.
 *
 * <p> This package provides a service clients can use to create and
 * authenticate a session. </p>
 *
 * <p> The service permits multiple variations of the number of
 * credential fields clients should provide and clients should
 * support this gracefully. It can choose not support all possible
 * combinations. Clients should assume that credentials that have
 * been marked hidden may contain information sensitive to the user
 * and they should process these credentials accordingly. </p>
 */

package remote.service.authentication;