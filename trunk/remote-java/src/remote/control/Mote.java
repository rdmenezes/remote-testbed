package remote.control;

/** Basic mote control interface. */
public interface Mote extends MoteInfo {
	//void setAttribute(String name, String value); // throws OperationNotSupported;

	void start();
	void stop();
	void reset();

	/** 
	 * Program the mote.
	 *
	 * @param binary The mote binary to use.
	 * @return		True if programming ...
	 */
	boolean program(String binary);

	void cancelProgramming();

	//void readConsole();
	void writeData(String data);

	MoteStatus getStatus();

        void release();

}