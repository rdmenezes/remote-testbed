package authentication;

import java.io.Serializable;

/** Authentication credential field.
 *
 * This class corresponds to one credential value that should be
 * exchanged between the client and server in order to authenticate the
 * client. Credentials containing secret values, such as passwords, can
 * be marked hidden.
 */
public class Credential implements Serializable{

	private static final long serialVersionUID = -1425162133341080154L;
	private String label, value;
	private boolean hidden;

	/** Create field with given label and hidden attribute.
	 *
	 * @param label		The field label.
	 * @param hidden	Whether the field should be hidden.
	 */
	public Credential(String label, boolean hidden)
	{
		super();
		this.label = label;
		this.value = null;
		this.hidden = hidden;
	}

	/** Dummy constructor to silence compiler. */
	public Credential()
	{
		this(null, false);
	}

	/** Is field value hidden?
	 *
	 * @return Whether the field is hidden.
	 */
	public boolean isHideValue()
	{
		return hidden;
	}

	/** Change hidden attribute of a field value.
	 *
	 * @param hidden	The new state of the hidden attribute.
	 */
	public void setHideValue(boolean hidden)
	{
		this.hidden = hidden;
	}

	/** Get field label.
	 *
	 * @return The field label.
	 */
	public String getLabel()
	{
		return label;
	}

	/** Set field label.
	 *
	 * @param label		The field label.
	 */
	public void setLabel(String label)
	{
		this.label = label;
	}

	/** Get field value.
	 *
	 * @return The field value.
	 */
	public String getValue()
	{
		return value;
	}

	/** Set field value.
	 *
	 * @param value	The new field value.
	 */
	public void setValue(String value)
	{
		this.value = value;
	}

}
