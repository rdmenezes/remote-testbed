package remote.service.motedata;

import remote.service.Service;
import remote.service.ServiceCallback;

/** Service for getting mote data.
 *
 * The mote data can be anything from availability information, mote
 * attributes such as MAC and net address, as well as site attributes
 * such as position.
 */
public interface MoteDataService extends Service {

	/** Get status information about all motes in the testbed.
	 *
	 * Get all info about the testbed motes including mote and
	 * location attributes in a single table. On success, the
	 * result object is a {@link remote.service.motedata.SimpleTable}
	 * with each row representing a mote and each column representing
	 * mote data.
	 *
	 * @param session	ID of the session.
	 * @param callback	Callback for receiving result or error.
	 */
	void getMoteData(String session, ServiceCallback callback);

}
