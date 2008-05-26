package remote.service;

/** Generic service listener.
 * 
 * An callback interface used for signaling success or failure of
 * a service call. Inspired by GWT's AsyncCallback and Axis2's
 * AxisCallback interface.
 */
public interface ServiceCallback {

	/** Success callback.
	 *
	 * This gets called when a result is successfully received.
	 *
	 * @param result	Result object.
	 */
	void onSuccess(Object result);

	/** Fault callback.
	 *
	 * This gets called when a fault is received.
	 * 
	 * @param result	Result object.
	 */
	void onFault(Object result);

	/** Error callback.
	 *
	 * This gets called ONLY when an internal processing exception occurs.
	 *
	 * @param caught	Caught exception.
	 */
	void onError(Throwable caught);

	/** Completion callback.
	 *
	 * This is called at the end of the service request
	 * no matter what happens, quite like a finally block.
	 */
	void onComplete();

}
