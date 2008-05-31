package remote.control;

/** Low-level mote command enumeration type. */
public interface MoteCommandType {
	int UNKNOWN = 0;
	int RESET   = 1;
	int START   = 2;
	int STOP    = 3;
	int PROGRAM = 4;
	int CANCEL  = 0;
}
