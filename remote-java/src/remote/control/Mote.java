package remote.control;

/** Basic mote control interface.
 *
 * @author fonseca
 */
public interface Mote {
	public static final String MACADDRESS = "macaddress";
	public static final String NETADDRESS = "netaddress";
	public static final String PLATFORM = "platform";

	void start();
	void stop();
	void reset();

	/** 
	 * Program the mote.
	 *
	 * @param binary The mote binary to use.
	 */
	boolean program(String binary);

	void cancelProgramming();

	//void readConsole();
	void writeData(String data);

	MoteStatus getStatus();

        String getAttribute(String name);
        void setAttribute(String name, String value); // throws OperationNotSupported;

	void getMoteControl();
	void dropMoteControl();
        void release();
}