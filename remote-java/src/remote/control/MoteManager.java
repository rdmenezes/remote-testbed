package remote.control;

import java.util.Iterator;

/**
 *
 */
public interface MoteManager {

	/** Iterator for all motes.
	 *
	 * @return		Iterator for all motes.
	 */
	Iterator getAllMotesIterator();

	/** Iterator for controlled motes.
	 *
	 * @return		Iterator for controlled motes.
	 */
	Iterator getControlledMotesIterator();

	//MoteControl getMoteControl(Mote mote, MoteListener listener);
	//MoteGroup getMote(String[] mac, MoteListener listener);
	//void reserveMote(MoteInfo mote);

	void controlMote(MoteInfo mote, MoteListener listener);

	//void release();
	void refresh();

}
