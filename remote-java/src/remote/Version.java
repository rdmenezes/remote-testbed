package remote;

/** Library version. */
public final class Version {

	/** Major version. */
	public final static int major = 0;

	/** Minor version. */
	public final static int minor = 1;

	/** Formatted version string.
	 * 
	 * This is a combination of major and minor version separated
	 * by a dot.
	 */
	public final static String string = String.format("%s.%s", major, minor);

	private Version()
	{
	}

}
