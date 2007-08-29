#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "FileDescriptor.h"
#include "Mote.h"
#include "Message.h"
#include "client_server/ClientMsg.h"
#include "MsgPayload.h"
#include "client_server/MsgSession.h"
#include "client_server/MsgClientRequest.h"
#include "client_server/MsgClientConfirm.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

namespace remote { namespace mcs {

using namespace protocols;
using namespace protocols::client_server;

class Mote;
typedef std::map<dbkey_t,Mote*> motemapbykey_t;

class Session;
typedef std::map<int,Session*> sessionmapbyfd_t;

/** Each instance of this class handles a single client connection.
 **/
class Session : public FileDescriptor
{
	public:
		/** Session constructor.
		 * \param p_fd File descriptor for the new client connection.
		 * \param p_sessionMap The client object will register itself in this map.
		 **/
		Session(int p_fd,sessionmapbyfd_t& p_sessionMap);
		/** Makes the session object release any controlled motes and the call its destructor.
		 * \param silent If true, any exceptions generated during clean-up will be caught an logged silently.
		**/
		void destroy(bool silent = false);
		/** Sends a confirm message for a mote to the remote client.
		 * \param mote_id Database key of the mote
		 * \param confirm Confirm message to encapsulate
		**/
		
		/** Clears all session records from the database **/
		static void resetDb();
		
		void confirm(dbkey_t mote_id, MsgPayload& confirm);
		/** Forces the session to drop control of a mote.
		 *\param mote_id Database key of the mote to free
		**/
		void freeMote(dbkey_t mote_id);
		/** Database key for this sessions session record **/
		dbkey_t session_id;
	protected:
		/** Session object destructor
		**/
		virtual ~Session();
		/** Handles an event on the socket of the remote client by invoking the appropriate event handler.
		 * \param events Event descriptor.
		**/
		void handleEvent(short events);
		
		/** Creates a new session database record upon connection and sends it to the client.
		 * The session is not initially authenticated. The authentication is performed
		 * externally and registered on the record.
		 * Any client attempting to send messages to the server must authenticate before 
		 * messages are accepted. Failure to do so will disconnect the client.
		**/
		void createSessionRecord();
		/** Deletes the session record upon end of session. **/
		void deleteSessionRecord();
		/** On the first invocation, performs a database check to see if the session
		 * is authenticated. Subsequent invocations only check the database if no
		 * positive result has been obtained yet.
		**/
		bool isAuthenticated();
		/** Handles a request directed to the mote server. Requests directed to a
		 * mote are directed through this method as well.
		 * \param request Request to handle
		**/
		void handleClientRequest(MsgClientRequest& request);		
		/** Handles a request directed to a specific mote. This method looks up the mote
		 * object in the map of controlled motes and passess the request to the mote object.
		 * \param mote_id Database key of the mote to receive the request.
		 * \param request Request to handle
		**/
		void handleMoteRequest(dbkey_t mote_id,MsgPayload& request);
		/** Attempts to take control of a mote.
		 * \param mote_id Database key of the mote to control.
		**/
		void getMoteControl(dbkey_t mote_id);
		/** Releases control of a mote.
		 * \param mote_id Database key of the mote to release
		**/
		void dropMoteControl(dbkey_t mote_id);

		/** Set to true when proper client authentication has been completed. **/
		bool authenticated;
		/** Map of motes currently controlled in this session.**/
		motemapbykey_t motes;
		/** Incoming message handler object. **/
		Message message_in;
		/** Session map that this session is registered in. **/
		sessionmapbyfd_t& sessionMap;

};

}}

#endif
