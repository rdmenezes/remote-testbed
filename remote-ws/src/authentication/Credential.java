package authentication;

import java.io.Serializable;

public class Credential implements Serializable{

	private static final long serialVersionUID = -1425162133341080154L;
	private String label, value;
	private boolean hideValue;

	public Credential() {
		super();
		// TODO Auto-generated constructor stub
	}

	public Credential(String label, boolean hideValue) {
		super();
		// TODO Auto-generated constructor stub
		this.label = label;
		this.value = null;
		this.hideValue = hideValue;
	}
	public boolean isHideValue() {
		return hideValue;
	}
	public void setHideValue(boolean hideValue) {
		this.hideValue = hideValue;
	}

	public String getLabel() {
		return label;
	}
	public void setLabel(String label) {
		this.label = label;
	}
	public String getValue() {
		return value;
	}
	public void setValue(String value) {
		this.value = value;
	}

}
