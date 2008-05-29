package remote.control;

import org.junit.Test;
import static org.junit.Assert.*;

public class MoteResultTest {

	MoteResult success = MoteResult.SUCCESS;
	MoteResult failure = MoteResult.FAILURE;

	@Test
	public void compareSuccess()
	{
		assertEquals(success, MoteResult.SUCCESS);
		assert success == MoteResult.SUCCESS;
	}

	@Test
	public void compareFailure()
	{
		assertEquals(failure, MoteResult.FAILURE);
		assert failure == MoteResult.FAILURE;
	}

	class Result {

		MoteResult result;

		Result(MoteResult result)
		{
			this.result = result;
		}

		MoteResult get()
		{
			return result;
		}

	}

	@Test
	public void container()
	{
		Result result1 = new Result(MoteResult.SUCCESS);
		Result result2 = new Result(MoteResult.FAILURE);

		assertEquals(result1.get(), MoteResult.SUCCESS);
		assertEquals(result2.get(), MoteResult.FAILURE);
	}

}
