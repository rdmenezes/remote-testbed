package authentication;

import java.sql.ResultSet;
import util.SQLHelper;

/** Basic session authenticator
 *
 * This class is supposed to be used together with the user model
 * supplied with the Re-Mote database scripts.
 */
public class Authenticator extends AbstractAuthenticator {

	/** Get an empty array of credential fields
	 *
	 * @return Empty fields for project, user, and password.
	 * @throws Exception
	 */
	public Credential[] getEmptyCredentials() throws Exception
	{
		Credential credentials[] = new Credential[3];

		credentials[0] = new Credential("Project", false);
		credentials[1] = new Credential("User", false);
		credentials[2] = new Credential("Password", true);

		return credentials;
	}

	/** Check if the client supplied project, user, and password are valid
	 *
	 * @param session_id    ID of the session.
	 * @param credentials   The credentials supplied by the client
	 *
	 * @return Whether the session could be authenticated.
	 * @throws Exception
	 */
	protected boolean checkCredentials(String session_id, Credential[] credentials) throws Exception
	{
		SQLHelper sql = null;
		String checkSQL = "select 1 from user u, project p, user_project up " +
		                  "where u.login='" + credentials[1].getValue() + "' " +
		                  "and u.password=md5('" + credentials[2].getValue() + "') " +
		                  "and p.name='" + credentials[0].getValue() + "' " +
		                  "and up.user_id=u.id " +
		                  "and up.project_id=p.id";

		try {
			sql = new SQLHelper();
			sql.openDB();
			return sql.retrieve(checkSQL).next();

		} finally {
			sql.closeDB();
		}
	}

}
