package moteaccess;

public class MoteAccess extends AbstractMoteAccess {

	/**
	 * No reservation system, so no reservations.
	 */
	protected boolean hasReservation(long mote_id, String session_id) {
		return false;
	}

}
