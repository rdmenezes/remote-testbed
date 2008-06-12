package remote;

/** Version utility. */
public final class Version implements Comparable {

	/** Library major version. */
	public static final int MAJOR = 0;

	/** Library minor version. */
	public static final int MINOR = 1;

	/** Library version string. */
	public static final String STRING = String.format("%s.%s", MAJOR, MINOR);

	/** Library version. */
	public static final Version LIBRARY = new Version(MAJOR, MINOR);

	/** Major version. */
	private final int major;

	/** Minor version. */
	private final int minor;

	/** Formatted version string.
	 * 
	 * This is a combination of major and minor version separated
	 * by a dot.
	 */
	private final String string;

	/** Create a version.
	 *
	 * @param major	The major version.
	 * @param minor	The minor version.
	 */
	public Version(int major, int minor)
	{
		this.major = major;
		this.minor = minor;
		this.string = String.format("%s.%s", major, minor);
	}

	/** Get major version.
	 *
	 * @return	The major version.
	 */
	public int getMajor()
	{
		return major;
	}

	/** Get minor version.
	 *
	 * @return	The minor version.
	 */
	public int getMinor()
	{
		return minor;
	}

	/** Get version string.
	 *
	 * @return	The version string.
	 */
	public String getString()
	{
		return string;
	}

	/** Check if the library supports a specific version.
	 * 
	 * @param arg0	The version with which to compare.
	 * @return	Negative number, zero, or positive number indicating
	 *		whether the specified object has a lower, equal, or
	 *		higher version.
	 */
	public int compareTo(Object arg0)
	{
		if (arg0 instanceof Version) {
			Version version0 = (Version) arg0;

			if (version0.major == major)
				return version0.minor - minor;
			return version0.major - major;
		}

		return 0;
	}

}
