/*
 * Created on Sep 18, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package gui;
import javax.swing.*;

import util.MoteControl;

import java.awt.BorderLayout;
/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MoteConsoleView extends JPanel {

	private static final long serialVersionUID = 2666563646974168408L;
	private JScrollPane scrollpane;
	private SimpleConsole text;

	public MoteConsoleView(MoteControl moteControl)
	{
		super(new BorderLayout());
		text = new SimpleConsole();
		text.attachOutputSink(moteControl.getOutputStream());
		moteControl.attachOutputStream(text.getSystemOut());
		add(text,BorderLayout.CENTER);
	}

	public void clear()
	{
		this.text.clear();
	}
}
