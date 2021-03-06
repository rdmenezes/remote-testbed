#include "mcs/Session.h"

namespace remote { namespace mcs {

Session::Session(int p_fd, sessionmapbyfd_t& p_clientMap )
        : FileDescriptor(p_fd),
          authenticated(false),
          message_in(),
          sessionMap(p_clientMap)
{
	sessionMap[fd] = this;
	setSendTimeout( fd,0,500000); // 0.5 second host send timeout should be adequate
	// set keepalive options
	setKeepAlive( fd, 3, 120, 30);
	createSessionRecord();
}

Session::~Session()
{
	Log::info("Closing client connection!");
	sessionMap.erase(fd);
	deleteSessionRecord();
}

void Session::destroy(bool silent)
{

	motemapbykey_t::iterator mi;
	// release any controlled motes
	for ( mi = motes.begin() ; mi != motes.end(); mi++ )
	{
		try
		{
			if (mi->second)	mi->second->dropSession(false);
		}
		catch (mysqlpp::Exception e) // don't throw exceptions while destructing
		{
			if (silent)
			{
				Log::error("Exception while dropping mote client: %s - record not updated!", e.what());
			}
			else
			{
				__THROW__(e.what());
			}
		}
	}

	delete this;
}

void Session::resetDb()
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Query reset = sqlConn.query();
	reset << "delete from session";
	reset.parse();
	reset.execute();
}

void Session::createSessionRecord()
{
	// create a session record for this connection
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Result res;
	mysqlpp::Query query = sqlConn.query();
	query << "insert into session(auth) values (0)";
	query.parse();
	mysqlpp::ResNSel sqlres = query.execute();
	if (sqlres.success)
	{
		session_id = sqlres.insert_id;
		Message message;
		MsgSession msgSession(session_id);
		ClientMsg msg(msgSession);
		message.sendMsg(fd,msg);
	}
	else
	{
		delete this;
		__THROW__ ("Failed to insert new session in database!\n");
	}
	query.reset();
}

void Session::deleteSessionRecord()
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Result res;
	mysqlpp::Query query = sqlConn.query();
	query << "delete from session where id = " << session_id;
	query.execute();
	query.reset();
	authenticated = false;
}

bool Session::isAuthenticated()
{
	if (authenticated) return true;
	// look up and verify the authentication in the database
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Result res;
	mysqlpp::Query query = sqlConn.query();

	query << "select auth from session where id = " << session_id;
	res = query.store();

	if ( res.num_rows() == 1 )
	{
		authenticated = res.at(0)["auth"];
	}
	query.reset();

	if (authenticated)
		Log::info("Client authenticated!");
	else
		Log::warn("Client not authenticated!");
	return authenticated;
}

void Session::freeMote(dbkey_t mote_id)
{
	Message message;
	motemapbykey_t::iterator mi;
	mi = motes.find(mote_id);
	if ( mi != motes.end() )
	{
		motes.erase(mi);
		MsgClientConfirm confirm( MSGCLIENTCOMMAND_DROPMOTECONTROL, SUCCESS, mote_id );
		ClientMsg clientMsg(confirm);
		message.sendMsg(fd,clientMsg);
	}
}

void Session::confirm(dbkey_t mote_id, MsgPayload& moteMsg)
{
	Message message;
	MsgClientConfirm confirm( MSGCLIENTCOMMAND_MOTEMESSAGE, SUCCESS, mote_id, moteMsg );
	ClientMsg clientMsg(confirm);

	try {
		message.sendMsg(fd,clientMsg);
	}
	catch (remote::protocols::MMSException e)
	{
		Log::error("Exception: %s", e.what());
		this->destroy();
	}
}

void Session::handleEvent(short events)
{
	if ( (events & POLLHUP) || (events & POLLNVAL) ) __THROW__ ("Client connection closed by peer!");
	if ( (events & POLLIN) || (events & POLLPRI) )
	{
		// no input is accepted before external authorization
		if ( !isAuthenticated() )
		{
			 __THROW__ ("Client not authenticated!\n");
		}

		// wait for an entire message before continuing
		if (!message_in.nonBlockingRecv(fd))
		{
			 return;
		}

		uint32_t msglen = message_in.getLength();
		uint8_t* buffer = message_in.getData();
		ClientMsg msg(buffer,msglen);

		switch (msg.getType())
		{
			case CLIENTMSG_CLIENTREQUEST:
			{
				MsgClientRequest& msgClientRequest = msg.getClientRequest();
				handleClientRequest(msgClientRequest);
				break;
			}
			default:
				__THROW__ ("Invalid message type from client!");
				break;
		}

	}
	else if ( (events & POLLERR) || (events & POLLHUP) || (events & POLLNVAL) )
	{
		__THROW__ ("Client connection closed.\n");
	}
}

void Session::handleClientRequest(MsgClientRequest& request)
{
	dbkey_t mote_id;
	motemapbykey_t::const_iterator mi;
	MsgMoteIdList& idlist = request.getMoteIdList();

	while( idlist.getNextMoteId(mote_id) )
	{
		// check the session id
		switch ( request.getCommand() )
		{
			case MSGCLIENTCOMMAND_GETMOTECONTROL:
                getMoteControl(mote_id);
				break;
			case MSGCLIENTCOMMAND_DROPMOTECONTROL:
                dropMoteControl(mote_id);
				break;
			case MSGCLIENTCOMMAND_MOTEMESSAGE:
				handleMoteRequest(mote_id,request.getMoteMsg());
				break;
			default:
				__THROW__ ("Got unknown request from client.\n");
				break;
		}
	}
}


void Session::handleMoteRequest(dbkey_t mote_id,MsgPayload& request)
{

	motemapbykey_t::const_iterator mi;

	mi = motes.find(mote_id);

	if ( mi != motes.end() )
	{
		mi->second->request(request);
	}
	else
	{
		MsgClientConfirm confirm( MSGCLIENTCOMMAND_MOTEMESSAGE, MOTE_NOT_CONTROLLED, mote_id );
		ClientMsg msg(confirm);
		Message message;
		message.sendMsg(fd,msg);
	}
}

void Session::getMoteControl(dbkey_t mote_id)
{
	Message message;
	Mote *mote;
	result_t result = FAILURE;

	mote = Mote::getById(mote_id);
	if (mote) {
		if (mote->setSession(this)) {
			motes[mote_id] = mote;
			result = SUCCESS;
		}
	}

	MsgClientConfirm confirm(MSGCLIENTCOMMAND_GETMOTECONTROL, result, mote_id);
	ClientMsg msg(confirm);
	message.sendMsg(fd, msg);
}

void Session::dropMoteControl(dbkey_t mote_id)
{
	Message message;
	motemapbykey_t::const_iterator mi;

	mi = motes.find(mote_id);

	if ( mi != motes.end() )
	{
		mi->second->dropSession();
		// dropSession should invoke freeMote that confirms the drop request
	}
	else
	{
		MsgClientConfirm confirm( MSGCLIENTCOMMAND_DROPMOTECONTROL, MOTE_NOT_CONTROLLED, mote_id );
		ClientMsg msg(confirm);
		message.sendMsg(fd,msg);
	}

}

}}
