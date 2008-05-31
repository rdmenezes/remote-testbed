package remote.control;

/** Low-level mote command enumeration type. */
public interface MoteCommandType {
	/** Start command. */
	int START   = 0;
	/** Stop command.*/
	int STOP    = 1;
	/** Reset command. */
	int RESET   = 2;
	/** Program command. */
	int PROGRAM = 3;
	/** Cancel command. */
	int CANCEL  = 4;

	/** Number of command types. */
	int TYPES   = 5;
}
