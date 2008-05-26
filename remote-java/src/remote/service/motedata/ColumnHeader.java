package remote.service.motedata;

import java.io.Serializable;

/** Table column header.
 *
 * Stores information about the data available in the
 * entire SimpleTable column.
 */
public class ColumnHeader implements Serializable {

	private String title;

	private String name;

	private boolean visible;

	private String valueclass;

	/** Create column header.
	 *
	 * @param title		The column title.
	 * @param name		The column name.
	 * @param visible	The column visibility.
	 * @param valueclass	The column valueclass.
	 */
	public ColumnHeader(String title, String name, boolean visible, String valueclass)
	{
		this.title = title;
		this.name = name;
		this.visible = visible;
		this.valueclass = valueclass;
	}

	/** Dummy constructor to silence compiler warnings. */
	public ColumnHeader()
	{
		this(null, null, false, null);
	}

	/** Get column header title.
	 *
	 * @return The column title. */
	public String getTitle()
	{
		return title;
	}

	/** Set column header title.
	 *
	 * @param title	The new column title.
	 */
	public void setTitle(String title)
	{
		this.title = title;
	}

	/** Get column header name.
	 *
	 * @return The column name.
	 */
	public String getName()
	{
		return name;
	}

	/** Set column header name.
	 *
	 * @param name	The new column name.
	 */
	public void setName(String name)
	{
		this.name = name;
	}

	/** Is the column header visible?
	 *
	 * @return Whether the column is visible.
	 */
	public boolean isVisible()
	{
		return visible;
	}

	/** Set column header visibility.
	 *
	 * @param visible The new column visibility.
	 */
	public void setVisible(boolean visible)
	{
		this.visible = visible;
	}

	/** Get value class of column header.
	 *
	 * @return The value class of the column.
	 */
	public String getValueclass()
	{
		return valueclass;
	}

	/** Set the column header value class.
	 *
	 * @param valueclass The column's new value class.
	 */
	public void setValueclass(String valueclass)
	{
		this.valueclass = valueclass;
	}

}
