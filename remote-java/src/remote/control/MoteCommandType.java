package remote.control;

/** Low-level mote command enumeration type. */
public interface MoteCommandType {
	/** Start command. */
	public static final int START   = 0;
	/** Stop command.*/
	public static final int STOP    = 1;
	/** Reset command. */
	public static final int RESET   = 2;
	/** Program command. */
	public static final int PROGRAM = 3;
	/** Cancel command. */
	public static final int CANCEL  = 4;
	/** Status command. */
	public static final int STATUS	= 5;
	/** Number of command types. */
	public static final int TYPES   = 6;
}
