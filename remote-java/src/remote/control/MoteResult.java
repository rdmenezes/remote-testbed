package remote.control;

/**
 *
 * @author fonseca
 */
public class MoteResult {
	final static int FAILURE = 0;
	final static int SUCCESS = 1;
	final static int WORKING = 2;
	final static int CANCELLED = 3;

	int result;

	int getResult()
	{
		return result;
	}
}
