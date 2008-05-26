package remote.control;

/**
 *
 */
public interface MoteListener extends java.util.EventListener {

	void startDone(Mote mote, MoteResult result, MoteStatus status);

	void stopDone(Mote mote, MoteResult result, MoteStatus status);

	void resetDone(Mote mote, MoteResult result, MoteStatus status);
	// cancelProgrammingDone() or cancelDone()?
	void programDone(Mote mote, MoteResult result, MoteStatus status);

	void readConsoleDone(Mote mote, String data /*, boolean more */);

	void writeConsoleDone(Mote mote);

	void setAttributeDone(Mote mote);

	void controlled(Mote mote);

	void lost(Mote mote, MoteResult result);

}
