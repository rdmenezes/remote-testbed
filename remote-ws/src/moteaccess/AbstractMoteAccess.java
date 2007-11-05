package moteaccess;

import java.sql.ResultSet;
import util.SQLHelper;

/** Generic base class for assigning mote usage privileges.
 *
 * This is an abstract class with the generic code to assign mote usage privileges.
 * To integrate any custom reservation system, this class must be inherited and the
 * abstract methods implemented.
 * @author zept
 */
public abstract class AbstractMoteAccess {

	private static final String ACCESS_QUERY =
		"update mote set priv_session_id=? where id=?";

	private static final String PRIV_QUERY =
		"select priv_session_id from mote where id=?";

	/** Acquire mote control privileges using specific session.
	 *
	 * @param mote_ids	IDs of mote to control.
	 * @param session_id	ID of session that wants privileges.
	 * @return A boolean array with entries for each requested mote.
	 * @throws Exception
	 */
	public boolean[] getMoteControlPrivileges(long[] mote_ids, String session_id) throws Exception
	{
		boolean[] results = new boolean[mote_ids.length];

		for (int i = 0; i < mote_ids.length; i++) {
			results[i] = getMoteControlPrivilege(mote_ids[i], session_id);
		}

		return results;
	}

	/** Acquire control privilege for one mote.
	 *
	 * @param mote_id	ID of mote to control.
	 * @param session_id	ID of session that wants control privilege.
	 * @return True if mote control privilege was granted.
	 * @throws Exception
	 */
	protected boolean getMoteControlPrivilege(long mote_id, String session_id) throws Exception
	{
		boolean access = false;
		SQLHelper sql = null;
		Object[] priv_params = { new Long(mote_id) };

		/* TODO: The queries in here are executed multiple
		 * times. Since we use PreparedStatements this could be
		 * optimized by moving the query initialization out of
		 * the loop. This would require that SQLHelper exposes
		 * more of its internal state such as allowing
		 * PreparedStatement to be returned. */
		try {
			sql = new SQLHelper();
			sql.openDB();

			/* Check for current privileged session. */
			ResultSet sqlres = sql.retrieve(PRIV_QUERY, priv_params);
			if (sqlres.next()) {
				String privsess = sqlres.getString("priv_session_id");

				access = checkMoteAccess(session_id, privsess, mote_id);
			}

			if (access) {
				/* Grant privileges to current session. */
				Object[] access_params = { session_id, new Long(mote_id) };
				sql.execute(ACCESS_QUERY, access_params);
			}
			return access;

		} finally {
			sql.closeDB();
		}
	}

	/** Check for mote access.
	 *
	 * Check if mote can be accessed by the given session, either
	 * because no session or the given session is (already) using
	 * the mote, or the give session has an active reservation.
	 *
	 * @param session_id	ID of session that wants access.
	 * @param priv_id	ID of currently privileged session; null if none.
	 * @param mote_id	ID of mote for which access is wanted.
	 * @return True if mote access can be acquired.
	 * @throws Exception
	 */
	protected boolean checkMoteAccess(String session_id, String priv_id, long mote_id) throws Exception
	{
		/* Check if mote is not in use or is already under control? */
		if (priv_id == null || priv_id.compareTo(session_id) == 0)
			return true;

		/* Check privileges with reservation system. */
		return hasReservation(mote_id, session_id);
	}

	/** Check if session can reserve a given mote.
	 *
	 * This method must be implemented to integrate a custom
	 * reservation system.  It is also a point for integration
	 * between user account system and reservation system.
	 *
	 * Note that the user model should specify the relevant
	 * reservation key upon session authentication. The details of
	 * how to do this are application specific.
	 *
	 * @param mote_id	Database key for the mote to check
	 * @param session_id	Database key for the session requesting access.
	 * @return True if there is a current reservation, false otherwise
	 * @throws Exception
	 */
	abstract protected boolean hasReservation(long mote_id, String session_id) throws Exception;
}
