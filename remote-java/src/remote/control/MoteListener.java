/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package remote.control;

/**
 *
 * @author fonseca
 */
public interface MoteListener {
	void startDone(Mote mote, MoteResult result, MoteStatus status);
	void stopDone(Mote mote, MoteResult result, MoteStatus status);
	void resetDone(Mote mote, MoteResult result, MoteStatus status);
	// cancelProgrammingDone()?
	void programDone(Mote mote, MoteResult result, MoteStatus status);

	//void getStatusDone(Mote mote, MoteStatus);

	void readConsoleDone(Mote mote, String data /*, boolean more */);
	void writeConsoleDone(Mote mote);

	void setAttributeDone(Mote mote);
        //getAttributeDone();

	void granted(Mote mote);
	void lost(Mote mote, MoteResult result);
}
