package util;

import java.sql.Connection;
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
	 * @param query	The query to execute.
	 * @return	The retrieved result set. May be null.
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

	/** Execute update query and retrieve results.
	 *
	 * @param query	The query to execute.
	 * @return	The retrieved result set. May be null.
	 */
	public ResultSet retrieveRW(String query)
	{
		ResultSet rs = null;
		try {
			Statement stmt = SQLConnection.createStatement(java.sql.ResultSet.TYPE_SCROLL_SENSITIVE,
								       java.sql.ResultSet.CONCUR_UPDATABLE);
			rs = stmt.executeQuery(query);
		} catch (Exception e) {
			System.out.println("Exception: " + e.toString());
			System.out.println("Query: " + query);
		}
		return rs;
	}

	/** Execute update query and return numbers of results.
	 *
	 * @param query	The query to execute.
	 * @return	The size of the resulting set.
	 */
	public int update(String query)
	{
		int count = 0;
		try {
			Statement stmt = SQLConnection.createStatement();
			count = stmt.executeUpdate(query);
		} catch (Exception e) {
			System.out.println("Exception: " + e.toString());
			System.out.println("Query: " + query);
		}
		return count;
	}

	/** Execute update query.
	 *
	 * @param query	The query to execute.
	 */
	public void execute(String query)
	{
		try {
			Statement stmt = SQLConnection.createStatement();
			stmt.execute(query);
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
		}
		catch (Exception e)
		{
			System.out.println("Exception: " + e.getMessage());
		}
	}
}
