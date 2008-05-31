package remote.control;

/** Mote command container.
 *
 * Wrapper class for low-level mote command type.
 */
public class MoteCommand {

	/** Reset the mote. */
	public static final MoteCommand RESET = new MoteCommand(MoteCommandType.RESET);
	/** Start the mote. */
	public static final MoteCommand START = new MoteCommand(MoteCommandType.START);
	/** Stop the mote. */
	public static final MoteCommand STOP = new MoteCommand(MoteCommandType.STOP);
	/** Program the mote. */
	public static final MoteCommand PROGRAM = new MoteCommand(MoteCommandType.PROGRAM);
	/** Cancel the last command. */
	public static final MoteCommand CANCEL = new MoteCommand(MoteCommandType.CANCEL);
	/** Unknown (last) command. */
	public static final MoteCommand UNKNOWN = new MoteCommand(MoteCommandType.TYPES);
	/** The low-level command value. */
	private int command;

	MoteCommand(int command)
	{
		this.command = command;
	}

}
