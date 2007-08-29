package util;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.sql.DataSource;

public class SQLHelper {

	private Connection SQLConnection;
	private DataSource datasource;

	public SQLHelper() throws Exception
	{
		// Obtain our environment naming context
		Context initCtx = new InitialContext();
		Context envCtx = (Context) initCtx.lookup("java:comp/env");

		// Look up our data source
		datasource = (DataSource) envCtx.lookup("jdbc/REMOTE");
	}

	public void openDB() throws Exception
	{
		SQLConnection = datasource.getConnection();
	}

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
