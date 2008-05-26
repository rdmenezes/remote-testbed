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

	/** Check if the library supports a specific version.
	 * 
	 * @param neededMajor	The needed major version.
	 * @param neededMinor	The needed minor version.
	 * @return		True if the library version is newer than
	 *			the needed major and minor version.
	 */
	public final static boolean supports(int neededMajor, int neededMinor)
	{
		return major >= neededMajor && minor >= neededMinor;
	}

	private Version()
	{
	}

}
