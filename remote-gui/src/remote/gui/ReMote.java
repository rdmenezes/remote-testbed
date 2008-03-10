package remote.gui;

import java.io.*;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Properties;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.KeyEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Event;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.*;

import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.*;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.JPanel;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JToolBar;
import javax.swing.JButton;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.JComboBox;
import javax.swing.*;

import net.infonode.docking.*;
import net.infonode.docking.util.*;
import net.infonode.docking.theme.*;
import net.infonode.docking.drop.DropFilter;
import net.infonode.docking.drop.DropInfo;
import net.infonode.docking.properties.*;
import net.infonode.gui.laf.InfoNodeLookAndFeel;
import net.infonode.properties.propertymap.ref.ThisPropertyMapRef;
import net.infonode.util.Direction;

import remote.motecontrol.client.Session;
import remote.motecontrol.client.SessionEvent;
import remote.motecontrol.client.SessionEventListener;
import remote.motecontrol.client.ConnectionInfo;
import remote.motecontrol.client.Mote;
import remote.motecontrol.client.MoteControlStatus;
import remote.motecontrol.client.MoteControlStatusListener;
import remote.motecontrol.client.SimpleMoteManager;

import remote.service.moteaccess.client.ClientMoteAccess;
import remote.service.motedata.client.MoteDataWrapper;
import remote.service.motedata.client.Table;
import remote.service.motedata.client.TableRow;
import remote.service.authentication.client.AuthenticationDialog;
import remote.service.authentication.client.AuthenticationListener;

import remote.gui.treeorderings.OrderingsAllMotesView;
import remote.gui.treeorderings.OrderingsControlledMotesView;
import remote.gui.treeorderings.TreeGroupingSelector;
import remote.gui.util.MoteControlRow;
import remote.gui.util.MoteControlTable;
import remote.gui.util.SortedTreeNode;
import remote.gui.util.SortedTreeNodeTransferable;
import remote.gui.util.TableBasedTree;
import remote.gui.util.TableRowOrdering;


public class ReMote extends JFrame implements SessionEventListener,AuthenticationListener {

	private static int VIEW_ALL_MOTES = 0;

	private static int VIEW_CONTROLLED_MOTES = 1;

	private static int VIEW_MOTE_MAP = 2;

	private View[] views = new View[4];

	private DockingWindowListener viewListener = null;

	private JMenuBar jJMenuBar = null;

	private JMenu helpMenu = null;


	private JMenuItem aboutMenuItem = null;

	private RootWindow rootWindow = null;

	private RootWindowProperties rootWindowProperties = null;

	private TabWindow moteTabView = null;


	private SplitWindow splitWindow1 = null;
	private SplitWindow splitWindow2 = null;
	private SplitWindow splitWindow3 = null;

	/**
	 * The currently applied docking windows theme
	 */
	private DockingWindowsTheme currentTheme = new ShapedGradientDockingTheme();

	private ViewMap viewMap = new ViewMap();

	private remote.service.authentication.client.AuthenticationDialog authenticationDialog = null;
	private remote.service.moteaccess.client.ClientMoteAccess clientMoteAccess = null;

	private Properties settings = null;

	private Session session = null;

	private ConnectionInfo currentConnection = null;

	private static final int ICON_SIZE = 8;

	/**
	 * Custom view icon.
	 */
	private static final Icon VIEW_ICON = new Icon() {
		public int getIconHeight() {
			return ICON_SIZE;
		}

		public int getIconWidth() {
			return ICON_SIZE;
		}

		public void paintIcon(Component c, Graphics g, int x, int y) {
			Color oldColor = g.getColor();

			g.setColor(new Color(70, 70, 70));
			g.fillRect(x, y, ICON_SIZE, ICON_SIZE);

			g.setColor(new Color(100, 230, 100));
			g.fillRect(x + 1, y + 1, ICON_SIZE - 2, ICON_SIZE - 2);

			g.setColor(oldColor);
		}
	};



	private JMenuItem quickHelpMenuItem;

	protected HashMap moteViews = new HashMap();

	private static class MoteView extends View {
		private static int nextID = 10;
		private int id;

		MoteView(String title, Icon icon, Component component) {
			super(title, icon, component);
			this.id = nextID++;
			this.getWindowProperties().setDockEnabled(true);
			this.getWindowProperties().setUndockEnabled(true);
			this.getWindowProperties().setDragEnabled(true);
		}

		/**
		 * Returns the view id.
		 *
		 * @return the view id
		 */
		public int getId() {
			return id;
		}
	}

	/**
	 * This method initializes jContentPane
	 *
	 * @return javax.swing.JPanel
	 */
	private RootWindow getRootWindow() {
		if (rootWindow == null) {

		/*	MixedViewHandler handler = new MixedViewHandler(viewMap,
					new ViewSerializer() {
						public void writeView(View view, ObjectOutputStream out)
								throws IOException {
							out.writeInt(((DynamicView) view).getId());
						}

						public View readView(ObjectInputStream in)
								throws IOException {
							return getDynamicView(in.readInt());
						}
					});*/

			rootWindow = DockingUtil.createRootWindow(viewMap, true);
			// Set gradient theme. The theme properties object is the super
			// object of our properties object, which
			// means our property value settings will override the theme values
			getRootWindowProperties().addSuperObject(currentTheme.getRootWindowProperties());

			// Our properties object is the super object of the root window
			// properties object, so all property values of the
			// theme and in our property object will be used by the root window
			rootWindow.getRootWindowProperties().addSuperObject(getRootWindowProperties());
			rootWindow.getWindowProperties().setCloseEnabled(false);

			// Enable the bottom window bar
			rootWindow.getWindowBar(Direction.LEFT).setEnabled(true);
		}
		return rootWindow;
	}



	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		// UIManager.setLookAndFeel(new InfoNodeLookAndFeel());
		ReMote application = new ReMote();
		application.show();
	}

	/**
	 * This is the default cons
	 * tructor
	 */
	public ReMote() {
		super();
		initialize();
	}

	/**
	 * This method initializes this
	 *
	 * @return void
	 */
	private void initialize() {
		IconResources.initialize();
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.getContentPane().add(getRootWindow(), BorderLayout.CENTER);
		this.setJMenuBar(getJJMenuBar());
		this.setSize(new Dimension(800, 580));
		this.setTitle("Re-Mote Control Client");
		this.connect();
	}

	private Session getSession() {
		if (session == null) {
			SimpleMoteManager mm = new SimpleMoteManager();
			session = new Session(mm);
			session.addSessionListener(this);
		}
		return session;
	}

	/**
	 * This method initializes jJMenuBar
	 *
	 * @return javax.swing.JMenuBar
	 */
	private JMenuBar getJJMenuBar() {
		if (jJMenuBar == null) {
			JMenu submenu;
			JMenuItem menuitem;
			jJMenuBar = new JMenuBar();
			// FILE MENU
			submenu = new JMenu("File");
			menuitem = new JMenuItem("Connect...");
			menuitem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					if (ReMote.this.getSession().isConnected())
					{
						getSession().disconnect();
					}
					connect();
				}
			});
			submenu.add(menuitem);
			menuitem = new JMenuItem("Exit");
			menuitem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					System.exit(0);
				}
			});
			submenu.add(menuitem);
			jJMenuBar.add(submenu);
			// WINDOW MENU
			submenu = new JMenu("Window");
			menuitem = new JMenuItem("Available motes");
			menuitem.addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					DockingWindow a = getAllMotesView();

					if (!a.isShowing()){
						DockingWindow b = getControlledMotesView();
						if (b.isShowing())
						{
							b.split(a,Direction.LEFT,0.5f);
						} else
						{
							b = getMoteMapView();
							if (b.isShowing())
							{
								b.split(a,Direction.UP,0.666f);
							} else
							{
								getRootWindow().getWindow().split(getAllMotesView(),Direction.LEFT,0.3f);
							}
						}
					}
				}
			});
			submenu.add(menuitem);
			menuitem = new JMenuItem("Controlled motes");
			menuitem.addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					DockingWindow a = getControlledMotesView();

					if (!a.isShowing()){
						DockingWindow b = getAllMotesView();
						if (b.isShowing())
						{
							b.split(a,Direction.RIGHT,0.5f);
						} else
						{
							b = getMoteMapView();
							if (b.isShowing())
							{
								b.split(a,Direction.UP,0.666f);
							} else
							{
								getRootWindow().getWindow().split(getAllMotesView(),Direction.LEFT,0.3f);
							}
						}
					}
				}
			});
			submenu.add(menuitem);
			menuitem = new JMenuItem("Mote map");
			menuitem.addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					DockingWindow a = getMoteMapView();

					if (!a.isShowing()){
						DockingWindow b = getAllMotesView();
						DockingWindow c = getControlledMotesView();
						if (b.isShowing() && c.isShowing()) {
							b = b.getWindowParent().getWindowParent();
						} else if (c.isShowing()) {
							b = c;
						}
						if (b.isShowing())
						{
							b.split(a,Direction.DOWN,0.66f);
						} else {
							b = getRootWindow().getWindow();
							if (b != null) {
								b.split(a,Direction.LEFT,0.3f);
							} else
							{
								getRootWindow().setWindow(a);
							}
						}
					}
				}
			});
			//submenu.add(menuitem);
			jJMenuBar.add(submenu);
		}
		return jJMenuBar;
	}

	/**
	 * This method initializes jMenu
	 *
	 * @return javax.swing.JMenu
	 */
	private JMenu getHelpMenu() {
		if (helpMenu == null) {
			helpMenu = new JMenu();
			helpMenu.setText("Help");
			helpMenu.add(getQuickHelpMenuItem());
			helpMenu.add(getAboutMenuItem());
		}
		return helpMenu;
	}

	private JMenuItem getAboutMenuItem() {
		if (aboutMenuItem == null) {
			aboutMenuItem = new JMenuItem();
			aboutMenuItem.setText("About Re-Mote...");
			aboutMenuItem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					JTextArea aboutText = new JTextArea("Re-Mote version 0.9 by Esben Zeuthen\n\n" +
						"This software uses InfoNode Docking Windows (http://www.infonode.net)\n\n" +
						"This software is free of charge and is available from http://www.sf.net/projects/re-mote");
					aboutText.setEditable(false);
					aboutText.setLineWrap(true);
					aboutText.setWrapStyleWord(true);
					JDialog aboutDialog = new JDialog(ReMote.this,"About Re-Mote",true);
					aboutDialog.add(aboutText,BorderLayout.CENTER);
					aboutDialog.setSize(200,200);
					aboutDialog.setVisible(true);
				}
			});
		}
		return aboutMenuItem;
	}

	private JMenuItem getQuickHelpMenuItem() {
		if (quickHelpMenuItem == null) {
			quickHelpMenuItem = new JMenuItem();
			//quickHelpMenuItem.setText("Quick help...");
			quickHelpMenuItem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {

				}
			});
		}
		return quickHelpMenuItem;
	}

	private View getAllMotesView() {
		try {
			View allMotesView = views[VIEW_ALL_MOTES];
			if (allMotesView == null) {
				TransferHandler moteTransferHandler = new TransferHandler() {
					/**
					 *
					 */
					private static final long serialVersionUID = -4315016105775737850L;

					public int getSourceActions(JComponent c) {
						return COPY;
					}

					public boolean canImport(JComponent comp,
							DataFlavor[] transferFlavors) {
						return false;
					}

					protected Transferable createTransferable(JComponent c) {

						if (c.getClass().equals(JTree.class)) {
							JTree tree = (JTree) c;
							return new SortedTreeNodeTransferable(
									getSelectedNodes(tree));
						}
						return null;
					}
				};

				ActionListener actionListener = new ActionListener() {

					public void actionPerformed(ActionEvent e) {
						String cmd = e.getActionCommand();
						if (cmd.equals("refresh")) {
							MoteDataWrapper.updateMoteList();
						}
					}
				};

				JToolBar jToolBar = new JToolBar();
				jToolBar.setFloatable(false);
				jToolBar.add(ReMote.createButton("refresh",
						IconResources.VIEW_REFRESH, "Refresh the mote list.",
						actionListener));

				JToolBar jBottomBar = new JToolBar();
				jBottomBar.setFloatable(false);
				JComboBox comboBoxes[] = new JComboBox[4];
				for (int i = 0; i < 4; i++) {
					comboBoxes[i] = new JComboBox();
					jBottomBar.add(comboBoxes[i]);
				}
				MoteDataWrapper.setSession(this.getSession());
				MoteDataWrapper.updateMoteList();
				Table table = MoteDataWrapper.getMoteTable();
				TableBasedTree treeModel = new TableBasedTree(table, "Root",
						new TableRowOrdering[] { OrderingsAllMotesView.BY_TOS });

				final JTree jTree = new JTree(treeModel);
				jTree.setRootVisible(false);
				jTree.setCellRenderer(new TableBasedTreeCellRenderer());
				jTree.setDragEnabled(true);
				jTree.setTransferHandler(moteTransferHandler);
				ToolTipManager.sharedInstance().registerComponent(jTree);

				JScrollPane scrollPane = new JScrollPane(jTree);
				JPanel jPanel = new JPanel(new BorderLayout());

				jPanel.add(jToolBar, BorderLayout.NORTH);
				jPanel.add(scrollPane, BorderLayout.CENTER);
				jPanel.add(jBottomBar, BorderLayout.SOUTH);
				allMotesView = new View("All motes", VIEW_ICON, jPanel);
				views[VIEW_ALL_MOTES] = allMotesView;

				new TreeGroupingSelector(treeModel,
						OrderingsAllMotesView.orderings, comboBoxes);
			}

			return allMotesView;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private View getMoteMapView() {
		try {
			View moteMapView = views[VIEW_MOTE_MAP];
			if (moteMapView == null)
			{
				// TODO: implement the mote map functionality
				moteMapView = new View("Topology map",VIEW_ICON,null);
				views[VIEW_MOTE_MAP] = moteMapView;
			}
			return moteMapView;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private View getControlledMotesView() {
		try {

			View motesView = views[VIEW_CONTROLLED_MOTES];
			if (motesView == null) {

				TransferHandler moteControlTransferHandler = new TransferHandler() {
					private DataFlavor myFlavor = new DataFlavor(
							SortedTreeNodeTransferable.mimeType);

					public int getSourceActions(JComponent c) {
						return COPY;
					}

					public boolean canImport(JComponent comp,
							DataFlavor[] transferFlavors) {
						try {
							for (int i = 0; i < transferFlavors.length; i++) {
								if (!myFlavor.equals(transferFlavors[i])) {
									return false;
								}
							}
						} catch (Exception e) {
							e.printStackTrace();
							return false;
						}
						return true;
					}

					protected Transferable createTransferable(JComponent c) {
						if (c.getClass().equals(JTree.class)) {
							JTree tree = (JTree) c;
							TreePath[] selectedPaths = tree.getSelectionModel()
									.getSelectionPaths();
							SortedTreeNode[] nodes = getSelectedNodes(tree);
							return new SortedTreeNodeTransferable(nodes);
						}
						return null;
					}

					public boolean importData(JComponent comp, Transferable t) {
						try {
							SortedTreeNode[] nodes = (SortedTreeNode[]) t
									.getTransferData(myFlavor);
							TableRow[] rows = getLeafValues(nodes);
							long[] ids = getMoteIds(rows);
							boolean[] grants = ReMote.this.getClientMoteAccess().getMoteControlPrivileges(ids,ReMote.this.session.getSessionId());
							for (int i=0; i<grants.length;i++)
							{
								if (!grants[i]){
									// FIXME: use a default mote identification string
									JOptionPane.showMessageDialog(ReMote.this,"Unable to get control privilege for mote with ID "+ids[i]+".");
								}
							}
							session.getMoteManager().requestMotes(ids);
						} catch (Exception e) {
							e.printStackTrace();
						}
						return true;
					}
				};

				MoteControlTable table = new MoteControlTable(session
						.getMoteManager().getMoteList());

				final TableBasedTree treeModel = new TableBasedTree(
						table,
						"Control",
						new TableRowOrdering[] { OrderingsControlledMotesView.BY_TOS });

				final JTree jTree = new JTree(treeModel);
				jTree.setRootVisible(true);
				jTree.setCellRenderer(new TableBasedTreeCellRenderer());
				jTree.setTransferHandler(moteControlTransferHandler);
				jTree.setDragEnabled(true);
				jTree.setToggleClickCount(0);
				ToolTipManager.sharedInstance().registerComponent(jTree);

				ActionListener actionListener = new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						int i;
						String cmd = e.getActionCommand();
						SortedTreeNode[] nodes = getSelectedNodes(jTree);
						TableRow[] rows = getLeafValues(nodes);
						long[] motes = getMoteIds(rows);
						SimpleMoteManager moteManager = (SimpleMoteManager) session
								.getMoteManager();
						if (cmd.equals("release")) {
							moteManager.releaseMotes(motes);

						} else if (cmd.equals("console")) {
							for (i = 0; i < rows.length; i++) {
								getMoteView((MoteControlRow) rows[i]);
							}
						} else if (cmd.equals("program")) {
							java.util.Properties settings = getSettings();
							try {
								JFileChooser fc = new JFileChooser(settings
										.getProperty("flashImagePath", "/"));
								fc.setFileFilter(new FlashImageFilter());
								int returnVal = fc
										.showOpenDialog(getRootWindow());

								if (returnVal == JFileChooser.APPROVE_OPTION) {
									File file = fc.getSelectedFile();
									FileInputStream fi = new FileInputStream(
											file);
									byte[] bytes = new byte[(int) file.length()];
									fi.read(bytes);
									moteManager.program(motes, bytes);
									fi.close();
									settings.setProperty("flashImagePath", file
											.getParent());
									saveSettings();
								}
							} catch (Exception ex) {
								ex.printStackTrace();
							}
						} else if (cmd.equals("start")) {
							moteManager.start(motes);
						} else if (cmd.equals("stop")) {
							moteManager.stop(motes);
						} else if (cmd.equals("reset")) {
							moteManager.reset(motes);
						}
					}

				};

				MouseListener ml = new MouseAdapter() {
					public void mousePressed(MouseEvent e) {
						TreePath selPath = jTree.getPathForLocation(e.getX(), e
								.getY());
						if (selPath != null && e.getClickCount() > 0) {
							TableRow[] rows = getLeafValues(new SortedTreeNode[] { (SortedTreeNode) selPath
									.getLastPathComponent() });
							for (int i = 0; i < rows.length; i++) {
								if (MoteControlRow.class
										.isAssignableFrom(rows[i].getClass())) {
									MoteControlRow row = (MoteControlRow) rows[i];
									if (e.getClickCount() == 1) {
										View moteView = (View) moteViews
												.get(row);
										if (moteView != null) {
											moteView.requestFocusInWindow();
											DockingWindow parent = moteView
													.getWindowParent();
											if (parent != null && TabWindow.class
													.isAssignableFrom(parent
															.getClass())) {
												TabWindow tab = (TabWindow) parent;
												tab
														.setSelectedTab(tab
																.getChildWindowIndex(moteView));
											}

										}
									} else if (e.getClickCount() == 2) {
										getMoteTabView().addTab(
												getMoteView(row));
									}
								}
							}
						}
					}
				};
				jTree.addMouseListener(ml);

				TreeModelListener treeModelListener = new TreeModelListener() {

					public void treeNodesChanged(TreeModelEvent e) {
						// TODO Auto-generated method stub

					}

					public void treeNodesInserted(TreeModelEvent e) {
						jTree.expandPath(e.getTreePath().getParentPath());
					}

					public void treeNodesRemoved(TreeModelEvent e) {
						// TODO Auto-generated method stub

					}

					public void treeStructureChanged(TreeModelEvent e) {
						// TODO Auto-generated method stub

					}

				};

				treeModel.addTreeModelListener(treeModelListener);

				JToolBar jBottomBar = new JToolBar();
				jBottomBar.setFloatable(false);
				JComboBox comboBoxes[] = new JComboBox[4];
				for (int i = 0; i < 4; i++) {
					comboBoxes[i] = new JComboBox();
					jBottomBar.add(comboBoxes[i]);
				}

				JToolBar jToolBar = new JToolBar();
				jToolBar.setFloatable(false);
				jToolBar.add(ReMote.createButton("release",
						IconResources.MOTE_RELEASE,
						"Stop using the selected mote(s).", actionListener));
				jToolBar.addSeparator();
				jToolBar.add(ReMote.createButton("console",
						IconResources.MOTE_CONSOLE,
						"Open console windows for the selected mote(s).",
						actionListener));
				jToolBar.addSeparator();
				jToolBar.add(ReMote.createButton("program",
						IconResources.FLASH_MOTE,
						"Upload a program to the selected mote(s).",
						actionListener));
				jToolBar.add(ReMote.createButton("start",
						IconResources.START_MOTE,
						"Start the selected mote(s).", actionListener));
				jToolBar.add(ReMote.createButton("stop",
						IconResources.STOP_MOTE, "Stop the selected mote(s).",
						actionListener));
				jToolBar.add(ReMote.createButton("reset",
						IconResources.RESET_MOTE,
						"Reset the selected mote(s).", actionListener));

				JScrollPane scrollPane = new JScrollPane(jTree);
				JPanel jPanel = new JPanel(new BorderLayout());
				jPanel.add(jToolBar, BorderLayout.NORTH);
				jPanel.add(scrollPane, BorderLayout.CENTER);
				jPanel.add(jBottomBar, BorderLayout.SOUTH);
				motesView = new View("Controlled motes", VIEW_ICON, jPanel);
				views[VIEW_CONTROLLED_MOTES] = motesView;

				new TreeGroupingSelector(treeModel,
						OrderingsControlledMotesView.orderings, comboBoxes);
			}
			return motesView;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private View getMoteView(final MoteControlRow moteControlRow) {
		MoteView moteView = null;
		try {
			moteView = (MoteView) moteViews.get(moteControlRow);
		} catch (Exception e) {
		}

		try {
			if (moteView == null) {

				Long id = (Long) moteControlRow.get("mote_id");
				final long[] moteid = { id.longValue() };
				final MoteConsoleView console = new MoteConsoleView(
						moteControlRow);

				final JLabel logFileLabel = new JLabel();

				ActionListener actionListener = new ActionListener() {

					public void actionPerformed(ActionEvent e) {
						Properties settings = getSettings();
						SimpleMoteManager moteManager = (SimpleMoteManager) getSession()
								.getMoteManager();
						try {
							String cmd = e.getActionCommand();
							if (cmd.equals("program")) {
								JFileChooser fc = new JFileChooser(settings
										.getProperty("flashImagePath", "/"));
								fc.setFileFilter(new FlashImageFilter());
								int returnVal = fc
										.showOpenDialog(getRootWindow());

								if (returnVal == JFileChooser.APPROVE_OPTION) {
									File file = fc.getSelectedFile();
									FileInputStream fi = new FileInputStream(
											file);
									byte[] image = new byte[(int) file.length()];
									fi.read(image);
									moteManager.program(moteid, image);
									fi.close();
									settings.setProperty("flashImagePath", file
											.getParent());
									saveSettings();
								}
							} else if (cmd.equals("cancelprogram")) {
								moteManager.cancelProgramming(moteid);
							} else if (cmd.equals("start")) {
								moteManager.start(moteid);
							} else if (cmd.equals("stop")) {
								moteManager.stop(moteid);
							} else if (cmd.equals("reset")) {
								moteManager.reset(moteid);
							} else if (cmd.equals("clear")) {
								console.clear();
							} else if (cmd.equals("log_local")) {
								JFileChooser fc = new JFileChooser(settings.getProperty("moteLogPath", "/"));
								if (fc.showSaveDialog(getRootWindow()) == JFileChooser.APPROVE_OPTION) {
									File logFile = fc.getSelectedFile();
									settings.setProperty("moteLogPath", fc.getCurrentDirectory().getPath());
									saveSettings();
									if (logFile.exists() && JOptionPane.showConfirmDialog(getRootWindow(),"The file \""+logFile.getPath()+"\" exists.\nAppend log at the end?")!=JOptionPane.OK_OPTION)
									{
										return;
									}
									moteControlRow.startLog(logFile);
									logFileLabel.setText(logFile.getPath());
								}
							} else if (cmd.equals("log_local_stop")) {
								if(moteControlRow.isLogging() && JOptionPane.showConfirmDialog(getRootWindow(),"Stop logging data?")==JOptionPane.OK_OPTION)
								{
									moteControlRow.stopLog();
									logFileLabel.setText("");
								}
							}

						} catch (Exception ex) {
							ex.printStackTrace();
						}
					}

				};
				String mac = moteControlRow.get("macaddress").toString();
				String tos = moteControlRow.get("tosaddress").toString();
				String site = moteControlRow.get("site").toString();

				JToolBar jToolBar = new JToolBar();
				jToolBar.setFloatable(false);
				jToolBar.add(ReMote.createButton("program",
						IconResources.FLASH_MOTE,
						"Upload a program to the mote.", actionListener));
				jToolBar.add(ReMote.createButton("cancelprogram",
						IconResources.CANCEL_FLASH_MOTE,
						"Cancel the programming of the mote.", actionListener));
				jToolBar.add(ReMote.createButton("start",
						IconResources.START_MOTE, "Start the mote.",
						actionListener));
				jToolBar.add(ReMote.createButton("stop",
						IconResources.STOP_MOTE, "Stop the mote.",
						actionListener));
				jToolBar.add(ReMote.createButton("reset",
						IconResources.RESET_MOTE, "Reset the mote.",
						actionListener));
				jToolBar.addSeparator();
				jToolBar.add(ReMote.createButton("log_local",
						IconResources.START_LOG,
						"Create a local output log for the mote.", actionListener));
				jToolBar.add(ReMote.createButton("log_local_stop",
						IconResources.STOP_LOG,
						"Stop local logging.", actionListener));
				jToolBar.addSeparator();
				jToolBar.add(ReMote.createButton("clear",
						IconResources.CONSOLE_CLEAR,
						"Clear the console window.", actionListener));

				JPanel jPanel = new JPanel(new BorderLayout());
				JPanel jBottomPanel = new JPanel(new BorderLayout());

				MoteLedsActivePanel ledsPanel = new MoteLedsActivePanel(
						moteControlRow.getMoteLeds(), moteControlRow.getMote()
								.getStatus());
				MoteStatusIndicator status = new MoteStatusIndicator(
						moteControlRow.getMote().getStatus());
				jBottomPanel.add(ledsPanel, BorderLayout.WEST);
				jBottomPanel.add(logFileLabel,BorderLayout.CENTER);
				jBottomPanel.add(status, BorderLayout.EAST);

				jPanel.add(jToolBar, BorderLayout.NORTH);
				jPanel.add(console, BorderLayout.CENTER);
				jPanel.add(jBottomPanel, BorderLayout.SOUTH);
				moteView = new MoteView(tos + " (" + mac + ")@" + site, VIEW_ICON,
						jPanel);
				moteViews.put(moteControlRow, moteView);
				viewMap.addView(moteView.getId(),moteView);
				moteControlRow.getMote().getControlStatus().addChangeListener(
						new MoteControlStatusListener() {

							public void moteControlStatusChange(Mote mote) {
								if (mote.getControlStatus().get() == MoteControlStatus.CONTROL_REVOKED) {
									// FIXME: notify user
									MoteView moteView = (MoteView)moteViews.get(moteControlRow);
									moteViews.remove(moteControlRow);
									viewMap.removeView(moteView.getId());
									rootWindow.removeView(moteView);
								}

							}

						});
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		moteView.setVisible(true);
		getMoteTabView().addTab(moteView);
		getMoteTabView().repaint();
		//getSplitWindow1().setWindows(getSplitWindow3(),getMoteTabView());
		getSplitWindow1().setWindows(getSplitWindow3(),getMoteTabView());
		return moteView;
	}

	private TabWindow getMoteTabView() {
		try {
			if (this.moteTabView == null) {
				DockingWindowAdapter dwAdapter = new DockingWindowAdapter(){

					public void windowClosing(DockingWindow arg0) throws OperationAbortedException {
						TabWindow tabWin = DockingUtil.getTabWindowFor(arg0);
						// FIXME: not pretty, but it works. InfoNode closes the tabwindow with the last tab
						if (tabWin.getChildWindowCount() == 1) throw new OperationAbortedException();
					}};
				TransferHandler moteControlTransferHandler = new TransferHandler() {
					/**
					 *
					 */
					private static final long serialVersionUID = 3809387851070369895L;
					private DataFlavor myFlavor = new DataFlavor(
							SortedTreeNodeTransferable.mimeType);

					public int getSourceActions(JComponent c) {
						return COPY;
					}

					public boolean canImport(JComponent comp,
							DataFlavor[] transferFlavors) {
						try {
							for (int i = 0; i < transferFlavors.length; i++) {
								if (!myFlavor.equals(transferFlavors[i])) {
									return false;
								}
							}
						} catch (Exception e) {
							e.printStackTrace();
							return false;
						}
						return true;
					}

					protected Transferable createTransferable(JComponent c) {
						return null;
					}

					public void addNode(SortedTreeNode node) {
						if (node.isLeaf()) {
							Object value = node.getValue();
							if (MoteControlRow.class.isAssignableFrom(value
									.getClass())) {
								MoteControlRow row = (MoteControlRow) value;
								getMoteView(row);
							}
						} else {
							for (int i = 0; i < node.getChildCount(); i++) {
								this.addNode(node.getChild(i));
							}
						}
					}

					public boolean importData(JComponent comp, Transferable t) {
						try {
							SortedTreeNode[] nodes = (SortedTreeNode[]) t
									.getTransferData(myFlavor);
							for (int i = 0; i < nodes.length; i++) {
								this.addNode(nodes[i]);
							}
						} catch (Exception e) {
							e.printStackTrace();
						}
						return true;
					}
				};

				this.moteTabView = new TabWindow();
				DockingWindowProperties properties = this.moteTabView
				.getWindowProperties();
				this.moteTabView.addListener(dwAdapter);

				// Our properties object is the super object of the root window
				// properties object, so all property values of the
				// theme and in our property object will be used by the root
				// window
	//			this.moteWindow.getRootWindowProperties().addSuperObject(
	//					rootWindowProperties);
				this.moteTabView.setTransferHandler(moteControlTransferHandler);
				this.moteTabView.
				getWindowProperties().getDropFilterProperties().setChildDropFilter(
						new DropFilter(){

							public boolean acceptDrop(DropInfo arg0) {
								return arg0.getWindow().getClass() == MoteView.class;
							}

						});

			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		Iterator it = moteViews.keySet().iterator();
		while(it.hasNext())
		{
			moteTabView.addTab((MoteView)moteViews.get(it.next()));
		}


		return this.moteTabView;
	}

/*	private TabWindow getMoteTabWindow() {
		if (this.moteTabWindow == null) {
			this.moteTabWindow = new TabWindow();
			DockingWindowProperties properties = this.moteTabWindow
					.getWindowProperties();
			properties.setCloseEnabled(false);
			properties.setUndockEnabled(false);
			properties.setDockEnabled(false);
			properties.setDragEnabled(false);
			properties.setMaximizeEnabled(false);
			properties.setMinimizeEnabled(false);
			properties.setRestoreEnabled(false);
			Iterator it = moteViews.keySet().iterator();
			while(it.hasNext())
			{
				moteTabWindow.addTab((MoteView)moteViews.get(it.next()));
			}

		}
		return this.moteTabWindow;
	}*/

	private void setDefaultLayout() {
		rootWindow.setWindow(getSplitWindow1());
	}

	private Properties getSettings() {
		try {
			if (settings == null) {
				settings = new Properties();
				File settingsFile = new File(".re-mote");

				if (settingsFile.exists()) {
					FileInputStream si = new FileInputStream(settingsFile);
					settings.load(si);
					si.close();
				}
			}
			return settings;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private void saveSettings() {
		try {
			File settingsFile = new File(".re-mote");
			if (settingsFile.exists())
				settingsFile.delete();
			settingsFile.createNewFile();

			FileOutputStream so = new FileOutputStream(settingsFile);
			settings.store(so, null);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void connect() {
		try {
			Properties settings = this.getSettings();
			String server = settings.getProperty("serverName");

			// FIXME: prompt for server name if null
			if (server == null) {
				server = "amigos30.diku.dk";
				settings.setProperty("serverName", server);
				saveSettings();
			}

			// FIXME: prompt the port number
			String portString = settings.getProperty("serverPort");
			if (portString == null) {
				portString = "10000";
				settings.setProperty("serverPort", portString);
				saveSettings();
			}
			int port = Integer.parseInt(portString);

			currentConnection = new ConnectionInfo(server, port);
			Session session = this.getSession();
			session.connect(currentConnection);
			if (session.isConnected()) {
				this.setDefaultLayout();
			} else {
				JOptionPane.showMessageDialog(this,"Could not connect to mote control server at "+server+".");
			}
		} catch (Exception ex) {
			JOptionPane.showMessageDialog(this, "Login error: "
					+ ex.getMessage());
			ex.printStackTrace();
		}
	}

	private void authenticate() {
//		if (authenticationDialog == null) {
			String server = this.session.getServerConnection().getConnectionInfo().getServerName();
			String session = this.session.getSessionId();
			authenticationDialog = new AuthenticationDialog(this, server,session, this.getSettings(),this);
			authenticationDialog.setLocationRelativeTo(this);
			authenticationDialog.toFront();
			authenticationDialog.setModal(true);
			authenticationDialog.setVisible(true);
		//}
	}

	public void eventFired(Session client, SessionEvent event) {
		switch (event.getId()) {
		case SessionEvent.AUTHENTICATE:
			this.authenticate();
			break;
		case SessionEvent.CONNECTED:
			// FIXME: do this somewhere else
			//ConnectionInfo c = currentConnection;

			//setTitle("Re-Mote - " + c.getProjectName() + "(" + c.getUserName()
			//		+ ")" + "@" + c.getServerName());
			break;
		case SessionEvent.DISCONNECTED:
			// FIXME: notify user
			break;
		}
	}

	private static JButton createButton(String command, Icon icon,
			String tooltip, ActionListener listener) {

		Insets buttonInsets = new Insets(0, 0, 0, 0);
		JButton button = new JButton(icon);
		button.setBorderPainted(false);
		button.setMargin(buttonInsets);
		button.setActionCommand(command);
		button.setToolTipText(tooltip);
		button.addActionListener(listener);
		return button;
	}

	private static SortedTreeNode[] getSelectedNodes(JTree jTree) {
		TreePath[] selectedPaths = jTree.getSelectionModel()
				.getSelectionPaths();
		if (selectedPaths == null) return null;
		SortedTreeNode[] nodes = new SortedTreeNode[selectedPaths.length];
		for (int i = 0; i < selectedPaths.length; i++) {
			nodes[i] = (SortedTreeNode) selectedPaths[i].getLastPathComponent();
		}
		return nodes;
	}

	public static void addNode(SortedTreeNode node, java.util.Vector rows) {
		if (node.isLeaf()) {
			Object value = node.getValue();
			if (TableRow.class.isAssignableFrom(value.getClass())) {
				rows.add(value);
			}
		} else {
			for (int i = 0; i < node.getChildCount(); i++) {
				addNode(node.getChild(i), rows);
			}
		}
	}

	private static TableRow[] getLeafValues(SortedTreeNode[] nodes) {
		if (nodes == null) return null;
		java.util.Vector rows = new java.util.Vector();
		int i;
		for (i = 0; i < nodes.length; i++) {
			addNode(nodes[i], rows);
		}
		TableRow[] tableRows = new TableRow[rows.size()];
		java.util.Iterator it = rows.iterator();
		i = 0;
		while (it.hasNext()) {
			tableRows[i++] = (TableRow) it.next();
		}
		return tableRows;
	}

	private static long[] getMoteIds(TableRow[] rows) {
		long[] ids = new long[rows.length];
		for (int i = 0; i < rows.length; i++) {
			try {
				Long id = (Long) rows[i].get("mote_id");
				if (id != null) {
					ids[i] = id.longValue();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return ids;
	}

	public SplitWindow getSplitWindow1() {
		if (splitWindow1 == null)
		{
			//splitWindow1 = new SplitWindow(true,0.3f,getSplitWindow2(),getMoteTabView());
			splitWindow1 = new SplitWindow(true,0.3f,getSplitWindow3(),getMoteTabView());
		}
		return splitWindow1;
	}

	public SplitWindow getSplitWindow2() {
		if (splitWindow2 == null)
		{
			splitWindow2 = new SplitWindow(false,0.5f,getSplitWindow3(),getMoteMapView());
		}
		return splitWindow2;
	}

	public SplitWindow getSplitWindow3() {
		if (splitWindow3 == null)
		{
			splitWindow3 = new SplitWindow(true,0.5f,getAllMotesView(),getControlledMotesView());
		}
		return splitWindow3;
	}

	private RootWindowProperties getRootWindowProperties() {
		if (rootWindowProperties == null)
		{
			rootWindowProperties = new RootWindowProperties();
			rootWindowProperties.getDockingWindowProperties().setMinimizeEnabled(false);
			rootWindowProperties.getDockingWindowProperties().setMaximizeEnabled(false);
			rootWindowProperties.getDockingWindowProperties().setUndockEnabled(false);
			rootWindowProperties.getDockingWindowProperties().setDragEnabled(false);
			rootWindowProperties.getDockingWindowProperties().getDropFilterProperties().setChildDropFilter(
					new DropFilter(){

						public boolean acceptDrop(DropInfo arg0) {
							return false;
						}

					});
		}
		return rootWindowProperties;
	}

	private remote.service.moteaccess.client.ClientMoteAccess getClientMoteAccess() {
		if (clientMoteAccess == null)
		{
			clientMoteAccess = new ClientMoteAccess(this.session.getServerConnection().getConnectionInfo().getServerName());
		}
		return clientMoteAccess;
	}



	public void Authentication(boolean result, String message) {
		if (!result){
			System.out.println("authentication failed");
			authenticate();
		} else {
			saveSettings();
		}
	}

} // @jve:decl-index=0:visual-constraint="10,10"
