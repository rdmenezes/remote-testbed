package remote;

import org.junit.*;
import static org.junit.Assert.*;

public class VersionTest {

	@Test
	public void string()
	{
		String version = String.format("%s.%s", Version.major, Version.minor);

		assertEquals(version, Version.string);
	}

	@Test
	public void supports()
	{
		assert Version.supports(Version.major, Version.minor);
		assert Version.supports(0, 0);
		assert Version.supports(Version.major + 1, 0) == false;
		assert Version.supports(Version.major, Version.minor + 1) == false;
	}

}
