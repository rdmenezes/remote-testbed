package moteaccess;

import java.sql.ResultSet;
import util.SQLHelper;

/**
 * @author zept
 * This is an abstract class with the generic code to assign mote usage privileges.
 * To integrate any custom reservation system, this class must be inherited and the
 * abstract methods implemented.
 */
public abstract class AbstractMoteAccess {

	public boolean[] getMoteControlPrivileges(long[] mote_ids, String session_id) throws Exception
	{
		boolean[] results = new boolean[mote_ids.length];
		for (int i=0; i<mote_ids.length;i++)
		{
			results[i] = getMoteControlPrivilege(mote_ids[i],session_id);
		}
		return results;
	}

	protected boolean getMoteControlPrivilege(long mote_id, String session_id) throws Exception
	{
		boolean access = false;
		SQLHelper sql = null;
		Exception ex = null;
		try {
			// Check for current privileged session
			String query = "select priv_session_id from mote " +
			               "where id="+mote_id;
			sql = new SQLHelper();
			sql.openDB();
			ResultSet sqlres = sql.retrieve(query);
			if (sqlres.next()){
				String privsess = sqlres.getString("priv_session_id");
				if (privsess == null || privsess.compareTo(session_id) == 0) {
					access = true;
				} else {
					// check privileges with reservation system
					if (hasReservation(mote_id,session_id))
					{
						access = true;
					}
				}
			}
			if (access) {
				query = "update mote set priv_session_id="+session_id+
				        " where id="+mote_id;
				sql.execute(query);
			}
		} catch (Exception e)
		{
			ex = e;
		} finally {
			sql.closeDB();
			if (ex != null) throw ex;
		}
		return access;
	}

	/**
	 * This method must be implemented to integrate a custom reservation system.
	 * It is also a point for integration between user account system and reservation system.
	 * @param mote_id Database key for the mote to check
	 * @param session_id Database key for the session requesting access. Note that
	 * the user model should specify the relevant reservation key upon session authentication.
	 * The details of how to do this are application specific.
	 * @return true if there is a current reservation, false otherwise
	 */
	abstract protected boolean hasReservation(long mote_id, String session_id) throws Exception;
}
