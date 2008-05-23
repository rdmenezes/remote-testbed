package remote.control;

/**
 *
 * @author fonseca
 */
public interface MoteManager {
	Mote getMote(String mac, MoteListener listener);

	//getMoteAvailability();
        void refresh();
}
