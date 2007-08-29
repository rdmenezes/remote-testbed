package authentication;

import java.sql.ResultSet;
import util.SQLHelper;

public class Authenticator extends AbstractAuthenticator {

	public Credential[] getEmptyCredentials() throws Exception
	{
		Credential credentials[] = new Credential[3];
		credentials[0] = new Credential("Project",false);
		credentials[1] = new Credential("User",false);
		credentials[2] = new Credential("Password",true);
		return credentials;
	}

	protected boolean checkCredentials(String session_id,	Credential[] credentials) throws Exception {
		String checkSQL = "select 1 from user u, project p, user_project up " +
		                  "where u.login='"+credentials[1].getValue()+"' " +
		                  "and u.password=md5('"+credentials[2].getValue()+"') " +
		                  "and p.name='"+credentials[0].getValue()+"' " +
		                  "and up.user_id=u.id " +
		                  "and up.project_id=p.id";

		SQLHelper sql = null;
		ResultSet rs = null;
		Exception ex=null;
		boolean result = false;

		try {
			sql = new SQLHelper();
			sql.openDB();
			rs = sql.retrieve(checkSQL);
			if (rs.next()) result = true;
		} catch (Exception e) {
			ex = e;
		} finally {
			sql.closeDB();
			if (ex != null) throw ex;
		}
		return result;
	}

}
