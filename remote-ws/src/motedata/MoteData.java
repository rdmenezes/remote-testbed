package motedata;

import java.sql.ResultSet;
import util.SQLHelper;

import motedata.ColumnHeader;
import motedata.SimpleTable;

public class MoteData {

	public MoteData() {
	}

	/** Builds a query to extract all info about connected motes
	 * including mote and site attributes into a single table.
	 *
	 * @param session_id Not used at this time
	 * @return A serialized RowSetDynaClass for the resultset
	 * @throws Exception
	 */
	public SimpleTable getMoteData(String session_id) throws Exception
	{
		String query,select;
		SQLHelper sql = null;
		Exception ex = null;
		ResultSet rs = null;
		SimpleTable result = null;

		try {
			sql = new SQLHelper();
			sql.openDB();
			query = " select m.id mote_id, m.site_id site_id,s.sitename site, " +
					"case when m.curr_session_id="+session_id+" then 'controlled' " +
			        "when isnull(m.curr_session_id) then 'available' else 'occupied' end mote_usage ";

			select = "select id, name from moteattrtype order by sortseq";
			rs = sql.retrieve(select);
			while (rs.next()) {
				query+=getMoteAttrSubquery(rs.getString("name"),rs.getString("id"));
			}
			select = "select id, name from siteattrtype order by sortseq";
			rs = sql.retrieve(select);
			while (rs.next()) {
				query+=getSiteAttrSubquery(rs.getString("name"),rs.getString("id"));
			}
			query+=" from mote m, site s " +
			       " where s.id=m.site_id";
			rs = sql.retrieve(query);
			rs.last();
			int rows = rs.getRow();
			int cols = rs.getMetaData().getColumnCount();
			result = new SimpleTable(rows,cols);
			rs.beforeFirst();

			for (int i=0;i<cols;i++){
				String title = rs.getMetaData().getColumnLabel(i+1);
				String name = rs.getMetaData().getColumnName(i+1);
				String classname = rs.getMetaData().getColumnClassName(i+1);
				result.getColumnHeaders()[i] = new ColumnHeader(title,name,true,classname);
			}

			int i = 0;
			while (rs.next()) {
				for (int j=0;j<cols;j++)
				{
					result.getData()[i][j] = rs.getObject(j+1);
				}
				i++;
			}
		} catch (Exception e)
		{
			ex = e;
		} finally {
			sql.closeDB();
			if (ex != null) throw ex;
		}
		return result;
	}

	private String getMoteAttrSubquery(String colname,String type_id)
	{
		return ", (select ma.val from moteattr ma, mote_moteattr mma" +
		        " where ma.moteattrtype_id="+type_id+
		        " and ma.id = mma.moteattr_id" +
		        " and mma.mote_id=m.id) "+colname+" ";
	}

	private String getSiteAttrSubquery(String colname,String type_id)
	{
		return ", (select sa.val from siteattr sa, site_siteattr ssa" +
		        " where sa.siteattrtype_id="+type_id+
		        " and sa.id = ssa.siteattr_id" +
		        " and ssa.site_id=s.id) "+colname+" ";
	}
}
