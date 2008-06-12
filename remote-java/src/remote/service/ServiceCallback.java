package remote.service;

/** Generic service listener.
 * 
 * An callback interface used for signaling success or failure of
 * a service call. Inspired by GWT's AsyncCallback and Axis2's
 * AxisCallback interface.
 * 
 * @param <T> Type of the service result object.
 * @see <a href="http://ws.apache.org/axis2/1_3/api/org/apache/axis2/client/async/AxisCallback.html">org.apache.axis2.client.async.AxisCallback</a>
 * @see <a href="http://google-web-toolkit.googlecode.com/svn/javadoc/1.4/com/google/gwt/user/client/rpc/AsyncCallback.html">com.google.gwt.user.client.rpc.AsyncCallback</a>
 */
public interface ServiceCallback<T> {

	/** Success callback.
	 *
	 * This gets called when a result is successfully received.
	 *
	 * @param result	Result object.
	 */
	void onSuccess(T result);

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
