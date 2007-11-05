package authentication;

import util.SQLHelper;

/** Basic session authenticator.
 *
 * This class is supposed to be used together with the user model
 * supplied with the Re-Mote database scripts.
 */
public class Authenticator extends AbstractAuthenticator {

	private static final int PROJECT = 0;
	private static final int USERNAME = 1;
	private static final int PASSWORD = 2;

	private static final String CHECK_CRED_QUERY =
		"select 1 from user u, project p, user_project up " +
		"where u.login=? and u.password=md5(?) and p.name=? " +
		"and up.user_id=u.id and up.project_id=p.id";

	/** Get an empty array of credential fields.
	 *
	 * @return Empty fields for project, user, and password.
	 * @throws Exception
	 */
	public Credential[] getEmptyCredentials() throws Exception
	{
		Credential credentials[] = new Credential[3];

		credentials[PROJECT]  = new Credential("Project", false);
		credentials[USERNAME] = new Credential("User", false);
		credentials[PASSWORD] = new Credential("Password", true);

		return credentials;
	}

	/** Check if the client supplied project, user, and password are valid.
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
		Object[] params = {
			credentials[USERNAME].getValue(),
			credentials[PASSWORD].getValue(),
			credentials[PROJECT].getValue(),
		};

		try {
			sql = new SQLHelper();
			sql.openDB();
			return sql.retrieve(CHECK_CRED_QUERY, params).next();

		} finally {
			sql.closeDB();
		}
	}

}
