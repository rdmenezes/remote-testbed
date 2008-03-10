package remote.motecontrol.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;

public class Connection {
	private Socket socket;
	private boolean open = false;
	public DataInputStream is;
	public DataOutputStream os;

	ConnectionInfo connectionInfo;

	public Connection(ConnectionInfo connectionInfo) throws Exception
	{
		this.connectionInfo = connectionInfo;
		open();
	}

	public void open() throws Exception
	{
		socket = new Socket(connectionInfo.getServerName(),connectionInfo.getPort());
		is = new DataInputStream(socket.getInputStream());
		os = new DataOutputStream(socket.getOutputStream());
		open = true;
	}

	public void close() throws Exception
	{
		socket.shutdownInput();
		socket.shutdownOutput();
		socket.close();
		open = false;
	}

	public boolean isOpen() {
		return open;
	}

	public ConnectionInfo getConnectionInfo() {
		return connectionInfo;
	}


}
