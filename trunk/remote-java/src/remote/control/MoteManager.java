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
	Iterator getMoteInfoIterator();

	/** Iterator for controlled motes.
	 *
	 * @return		Iterator for controlled motes.
	 */
	Iterator getMoteControlIterator();

	//MoteControl getMoteControl(Mote mote, MoteListener listener);
	//MoteGroup getMote(String[] mac, MoteListener listener);
	//void reserveMote(MoteInfo mote);

	void controlMote(MoteInfo mote, MoteControlListener listener);

	//void release();
	//getMoteAvailability();
	void refresh();

}
