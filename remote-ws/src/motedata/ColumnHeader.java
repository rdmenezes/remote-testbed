package motedata;

import java.io.Serializable;

public class ColumnHeader implements Serializable
{
	/**
	 *
	 */
	private static final long serialVersionUID = -4526017460513320190L;
	private String title;
	private String name;
	private boolean visible;
	private String valueclass;

	public ColumnHeader()
	{
		title = null;
		name = null;
		visible = false;
		valueclass = null;
	}

	public ColumnHeader(String title, String name, boolean visible, String valueclass) {
		super();
		// TODO Auto-generated constructor stub
		this.title = title;
		this.name = name;
		this.visible = visible;
		this.valueclass = valueclass;
	}

	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public boolean isVisible() {
		return visible;
	}
	public void setVisible(boolean visible) {
		this.visible = visible;
	}

	public String getValueclass() {
		return valueclass;
	}

	public void setValueclass(String valueclass) {
		this.valueclass = valueclass;
	}

}
