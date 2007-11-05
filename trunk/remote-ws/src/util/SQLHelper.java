package util;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.sql.DataSource;

/** Database helper class. */
public class SQLHelper {

	private Connection SQLConnection;
	private DataSource datasource;

	/** Create managed database connection.
	 *
	 * @throws Exception
	 */
	public SQLHelper() throws Exception
	{
		// Obtain our environment naming context
		Context initCtx = new InitialContext();
		Context envCtx = (Context) initCtx.lookup("java:comp/env");

		// Look up our data source
		datasource = (DataSource) envCtx.lookup("jdbc/REMOTE");
	}

	/** Open connection to the database.
	 *
	 * @throws Exception
	 */
	public void openDB() throws Exception
	{
		SQLConnection = datasource.getConnection();
	}

	/** Execute query and retrieve results.
	 *
	 * Note, this should only be used for internal and sanitized
	 * parameters. Else use the version that takes parameters as
	 * a separate argument to prevent SQL injection attacks.
	 *
	 * @param query	 The query to execute.
	 * @return	 The retrieved result set. May be null.
	 * @see SQLHelper#retrieve(String query, Object[] params)
	 */
	public ResultSet retrieve(String query)
	{
		ResultSet rs = null;
		try {
			Statement stmt = SQLConnection.createStatement();
			rs = stmt.executeQuery(query);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Query: " + query);
		}
		return rs;
	}

	/** Execute query and retrieve results.
	 *
	 * @param query	 The query to execute.
	 * @param params The query parameters to use.
	 * @return	 The retrieved result set. May be null.
	 */
	public ResultSet retrieve(String query, Object[] params)
	{
		try {
			PreparedStatement stmt = SQLConnection.prepareStatement(query);

			for (int i = 0; i < params.length; i++) {
				stmt.setObject(i + 1, params[i]);
			}

			return stmt.executeQuery();

		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Query: " + query);
		}

		return null;
	}

	/** Execute update query and return numbers of results.
	 *
	 * @param query	 The query to execute.
	 * @param params The query parameters to use.
	 * @return	 The size of the resulting set.
	 */
	public int update(String query, Object[] params)
	{
		try {
			PreparedStatement stmt = SQLConnection.prepareStatement(query);

			for (int i = 0; i < params.length; i++) {
				stmt.setObject(i + 1, params[i]);
			}

			return stmt.executeUpdate();

		} catch (Exception e) {
			System.out.println("Exception: " + e.toString());
			System.out.println("Query: " + query);
		}

		return 0;
	}

	/** Execute update query.
	 *
	 * @param query	 The query to execute.
	 * @param params The query parameters to use.
	 */
	public void execute(String query, Object[] params)
	{
		try {
			PreparedStatement stmt = SQLConnection.prepareStatement(query);

			for (int i = 0; i < params.length; i++) {
				stmt.setObject(i + 1, params[i]);
			}

			stmt.execute();

		} catch (Exception e) {
			System.out.println("Exception: " + e.toString());
			System.out.println("Query: " + query);
		}
	}

	/** Close database connection. */
	public void closeDB()
	{
		try {
			SQLConnection.close();

		} catch (Exception e) {
			System.out.println("Exception: " + e.getMessage());
		}
	}
}
