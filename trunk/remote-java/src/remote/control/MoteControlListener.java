package remote.control;

/**
 *
 */
public interface MoteControlListener extends java.util.EventListener {

	void startDone(MoteControl mote, MoteResult result, MoteStatus status);

	void stopDone(MoteControl mote, MoteResult result, MoteStatus status);

	void resetDone(MoteControl mote, MoteResult result, MoteStatus status);
	// cancelProgrammingDone() or cancelDone()?
	void programDone(MoteControl mote, MoteResult result, MoteStatus status);

	void readConsoleDone(MoteControl mote, String data /*, boolean more */);

	void writeConsoleDone(MoteControl mote);

	void setAttributeDone(MoteControl mote);

	void controlled(MoteControl mote);

	void lost(MoteControl mote, MoteResult result);

}
