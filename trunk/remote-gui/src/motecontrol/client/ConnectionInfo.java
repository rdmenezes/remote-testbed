package diku.distlab.motecontrolclientlib.client;

public class ConnectionInfo {
	private String serverName;
	private int port;

	public ConnectionInfo(String serverName, int port) {
		super();
		// TODO Auto-generated constructor stub
		this.serverName = serverName;
		this.port = port;
	}

	public String getServerName() {
		return serverName;
	}

	public void setServerName(String serverName) {
		this.serverName = serverName;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

}
