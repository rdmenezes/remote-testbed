package remote.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.ListIterator;

import java.awt.BorderLayout;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.text.Document;

public class SimpleConsole extends JPanel {
	JTextArea outputArea;

	JTextField inputField;

	OutputStream userOutput;

	ArrayList history;

	int historyIndex = 0, historyOffset = 0;

	String buffer = "";

	Object lock = new Object();

	private ActionListener timerListener = new ActionListener() {

		public void actionPerformed(ActionEvent e) {

			synchronized (lock) {
				int nl = 0, cr;

				String prefix;
				cr = buffer.indexOf('\r');
				try {
					while (cr >= 0) {
						prefix = buffer.substring(0, cr);
						outputArea.append(prefix);
						nl = outputArea.getLineStartOffset(outputArea
								.getLineCount() - 1);
						outputArea.replaceRange(null, nl, outputArea.getText()
								.length());
						buffer = buffer.substring(cr + 1);
						cr = buffer.indexOf('\r');
					}
					outputArea.append(buffer);
					outputArea.setCaretPosition(outputArea.getText().length());
					buffer = "";
				} catch (Exception ex) {
					ex.printStackTrace();
				} finally {
					lock.notify();
				}
			}
		}

	};

	final MyTimer timer = new MyTimer(timerListener);

	static final int historyLimit = 1000;

	private OutputStream systemOutput = new OutputStream() {

		public void write(int b) throws IOException {
			synchronized (lock) {
				buffer += new String(new byte[] { (byte) b });
				if (!timer.isRunning())
					timer.run();
				lock.notify();
			}
		}

		public void write(byte[] b, int off, int len) throws IOException {
			synchronized (lock) {
				buffer += new String(b, off, len);
				if (!timer.isRunning())
					timer.run();
				lock.notify();
			}
		}
	};

	private static class MyTimer extends Timer implements Runnable {
		MyTimer(ActionListener l) {
			super(20, l);
			setRepeats(false);
		}

		public void run() {
			synchronized (this) {
				start();
			}
		}

		void startLater() {
			SwingUtilities.invokeLater(this);
		}
	}

	private ActionListener actionListener = new ActionListener() {

		public void actionPerformed(ActionEvent e) {
			String input = inputField.getText();

			history.add(historyIndex, input);
			historyIndex = (historyIndex + 1) % historyLimit;
			historyOffset = 0;
			if (userOutput != null && input.length() > 0) {
				input += "\n";
				try {
					userOutput.write(input.getBytes("ISO-8859-1"));
				} catch (Exception ex) {
					ex.printStackTrace();
				}
			}

			inputField.setText(null);
		}

	};

	private KeyListener keyListener = new KeyListener() {
		public void keyTyped(KeyEvent e) {

		}

		public void keyPressed(KeyEvent e) {
			switch (e.getKeyCode()) {
			case KeyEvent.VK_UP:

				historyOffset--;

				if (-historyOffset > history.size())
					historyOffset = -history.size();

				if (historyOffset < 0) {
					inputField.setText((String) history.get(historyIndex
							+ historyOffset));
				}
				break;
			case KeyEvent.VK_DOWN:

				historyOffset++;

				if (historyOffset > 0)
					historyOffset = 0;

				if (historyOffset < 0) {
					inputField.setText((String) history.get(historyIndex
							+ historyOffset));
				} else {
					inputField.setText(null);
				}

				break;
			default:
				break;
			}

		}

		public void keyReleased(KeyEvent e) {
			// TODO Auto-generated method stub

		}

	};

	public SimpleConsole() {
		super(new BorderLayout());

		outputArea = new JTextArea();
		outputArea.setEditable(false);
		outputArea.setLineWrap(true);

		inputField = new JTextField();
		inputField.addActionListener(actionListener);
		inputField.addKeyListener(keyListener);

		history = new ArrayList();

		this.add(new JScrollPane(outputArea,
				JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER), BorderLayout.CENTER);
		this.add(inputField, BorderLayout.SOUTH);

	}

	/** Provide a stream for the console to output user input * */
	public void attachOutputSink(OutputStream userOutput) {
		this.userOutput = userOutput;
	}

	/** Get the outputstream for writing mote console data to the console window * */
	public OutputStream getSystemOut() {
		return this.systemOutput;
	}

	public void clear()
	{
		this.outputArea.setText(null);
	}
}
