/*
 * Created on Oct 3, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package diku.distlab.remote.client.authentication;


import javax.swing.*;

import diku.distlab.remote.authentication.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.*;
import java.util.Observable;
import java.util.Observer;
import java.util.Properties;
/**
 * @author zept
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class AuthenticationDialog extends JDialog {

	/**
	 *
	 */
	private static final long serialVersionUID = -2677961240523912621L;
	private ClientAuthenticator authenticator;
	private Credential credentials[];
	private JLabel heading;
	private JLabel labels[];
	private JTextField fields[];
	private Container panel;
	private JButton bOK,bQuit;


	public AuthenticationDialog(JFrame f, final String server,
				    final String session_id,
				    final Properties settings,
				    final AuthenticationListener authlistener)
	{
		super(f,"Authenticate...",true);
		final AuthenticationDialog dialog = this;
		authenticator = new ClientAuthenticator(server);
		ActionListener listener = new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				dialog.dispose();
				String c = e.getActionCommand();
				if (c.equals("authenticate")) {
					for (int i=0; i<credentials.length;i++)
					{
						credentials[i].setValue(fields[i].getText());
					}

					boolean authenticated = false;
					try {
						authenticated = authenticator.authenticate(session_id, credentials);
					} catch (Exception ex)
					{
						ex.printStackTrace();
					}

					if (authenticated) {
						for (int i = 0; i < credentials.length; i++) {
							if (credentials[i].isHideValue())
								continue;
							String name = "auth-" + server + "-" + credentials[i].getLabel();
							settings.setProperty(name, credentials[i].getValue());
						}
					}
					authlistener.Authentication(authenticated, "");

				} else if (c.equals("quit")) {
					System.exit(0);
				}

			}
		};

		panel = getContentPane();
		GridBagLayout gl = new GridBagLayout();
		panel.setLayout(gl);

		heading = new JLabel("Specify credentials for " + server);
		heading.setHorizontalAlignment(JLabel.CENTER);

		bOK = new JButton("Log on...");
		bOK.setActionCommand("authenticate");
		bOK.addActionListener(listener);

		bQuit = new JButton("Quit");
		bQuit.setActionCommand("quit");
		bQuit.addActionListener(listener);

		setResizable(false);
		setSize(400,290);

		GridBagConstraints c = new GridBagConstraints();

		c.fill = GridBagConstraints.HORIZONTAL;
		c.insets = new Insets(10,15,0,15);

		c.gridx = 1;
		c.gridwidth = 8;

		panel.add(heading,c);

		int i;
		try
		{
			credentials = authenticator.getEmptyCredentials();
		} catch (Exception ex)
		{
			ex.printStackTrace();
		}
		labels = new JLabel[credentials.length];
		fields = new JTextField[credentials.length];
		for (i=0; i<credentials.length;i++){
			labels[i] = new JLabel(credentials[i].getLabel());
			if (credentials[i].isHideValue()){
				JPasswordField pw = new JPasswordField();
				fields[i] = pw;
			} else	{
				String name = "auth-" + server + "-" + credentials[i].getLabel();
				String value = settings.getProperty(name);
				JTextField field = new JTextField();
				field.setEditable(true);
				if (value != null)
					field.setText(value);
				fields[i] = field;
			}
			c.weightx = 0.3;
			c.gridy = 1+2*i;
			panel.add(labels[i],c);
			c.gridx = 1;
			c.insets = new Insets(0,15,0,15);
			c.weightx = 0.8;
			c.gridy = 2+2*i;
			panel.add(fields[i],c);

		}

		c.gridy = 2*credentials.length+2;
		c.fill = GridBagConstraints.NONE;
		c.anchor = GridBagConstraints.LINE_END;

		c.weightx = 0;
		c.gridwidth = GridBagConstraints.RELATIVE;
		c.gridx = 7;
		c.insets = new Insets(20,15,0,15);
		panel.add(bOK,c);
		c.gridx=8;
		panel.add(bQuit,c);
	}
}
