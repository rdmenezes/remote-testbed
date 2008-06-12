package remote.service.motecontrol;

import remote.service.Service;
import remote.service.ServiceCallback;

/** Service for controlling motes. */
public interface MoteControlService extends Service {

	/** Get status for motes.
	 *
	 * On success, the result object is an array of {@link MoteControlResult}
	 * objects (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes for which to get status.
	 * @param callback	Callback for receiving result or error.
	 */
	void getStatus(String session, long[] motes,
		       ServiceCallback<MoteControlResult[]> callback);

	/** Start motes.
	 *
	 * On success, the result object is an array of {@link MoteControlResult}
	 * objects (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes to control.
	 * @param callback	Callback for receiving result or error.
	 */
	void start(String session, long[] motes,
		   ServiceCallback<MoteControlResult[]> callback);

	/** Stop motes.
	 *
	 * On success, the result object is an array of {@link MoteControlResult}
	 * objects (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes to control.
	 * @param callback	Callback for receiving result or error.
	 */
	void stop(String session, long[] motes,
		  ServiceCallback<MoteControlResult[]> callback);

	/** Reset motes.
	 *
	 * On success, the result object is an array of {@link MoteControlResult}
	 * objects (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes to control.
	 * @param callback	Callback for receiving result or error.
	 */
	void reset(String session, long[] motes,
		   ServiceCallback<MoteControlResult[]> callback);

	/** Program motes.
	 *
	 * On success, the result object is an array of {@link MoteControlResult}
	 * objects (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes to control.
	 * @param program	Binary to use when programming.
	 * @param callback	Callback for receiving result or error.
	 */
	void program(String session, long[] motes, String program,
		     ServiceCallback<MoteControlResult[]> callback);

	/** Cancel last operation.
	 *
	 * Can be used to cancel a previous programming request
	 * if it takes too long. On success, the result object
	 * is an array of {@link MoteControlResult} objects
	 * (MoteControlResult[]) with an entry for each mote.
	 *
	 * @param session	ID of the session that wants privileges.
	 * @param motes		IDs of motes to control.
	 * @param callback	Callback for receiving result or error.
	 */
	void cancel(String session, long[] motes,
		    ServiceCallback<MoteControlResult[]> callback);

}
