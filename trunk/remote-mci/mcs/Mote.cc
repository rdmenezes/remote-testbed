#include "Mote.h"
#include "macros.h"

namespace remote { namespace mcs {
	
Mote::Mote( dbkey_t p_mote_id,
		    dbkey_t p_site_id,
		    MoteControlInfrastructure& p_messenger,
            MCIAddress& p_mciAddress)
      : mciAddress(p_mciAddress),
        mote_id(p_mote_id),
        site_id(p_site_id),
        mci(p_messenger),        
        session(NULL)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	// look up the mote record in the database and make sure it corresponds
	// to the supplied information
	mysqlpp::Query moteupdate = sqlConn.query();
	moteupdate << "update mote set site_id = %0:site_id, curr_session_id=NULL, priv_session_id=NULL where id = %1:mote_id";
	moteupdate.parse();
	
	moteupdate.def["site_id"] = site_id;
	moteupdate.def["mote_id"] = mote_id;
	mysqlpp::ResNSel sqlres = moteupdate.execute();
	
	if ( sqlres.success && sqlres.rows == 1) {
		// register the mote in the mote pool	
		registerMote(this);
	} else	{
		log("Info: %s, site_id: %u, mote_id: %u\n",sqlres.info.c_str(),site_id,mote_id);
		delete this;
		__THROW__ ("Unable to register site on mote!\n");
	} 
}

Mote::Mote( dbkey_t p_site_id,
		    MoteControlInfrastructure& p_messenger,
		    MCIAddress& p_mciAddress )
     : mciAddress(p_mciAddress),
       mote_id(0),
       site_id(p_site_id),
       mci(p_messenger),
       session(NULL)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	// create a new mote with the supplied site_id
	mysqlpp::Query moteinsert = sqlConn.query();
	moteinsert << "insert into mote(site_id) values( %0:site_id )";
	moteinsert.parse();
	moteinsert.def["site_id"] = site_id;
	mysqlpp::ResNSel sqlres = moteinsert.execute();	
	if (sqlres.success)
	{
		mote_id = sqlres.insert_id;
		// register the mote in the mote pool	
		registerMote(this);
	//	stop();
	}
	else
	{
		delete this;
		__THROW__ ("Failed to insert new mote in database!\n");
	}
}


Mote::~Mote()
{
	mote.erase(mote_id);
}

void Mote::destroy(bool silent)
{
	try {
		dropSession();	
		mysqlpp::Connection& sqlConn = dbConn.getConnection();
		mysqlpp::Query moteupdate = sqlConn.query();
		moteupdate << "update mote set site_id = NULL where id = %1:mote_id";
		moteupdate.parse();
		moteupdate.def["mote_id"] = mote_id;
		moteupdate.execute(); // no problem even if mote record does not exist	
	}
	catch (mysqlpp::Exception e)
	{
		if (silent)
		{
			log("Exception while updating mote record: %s - record not updated!\n",e.what());
		}
		else
		{
			__THROW__(e.what());
		}		
	}
	delete this;
}

void Mote::request( MsgPayload& request)
{
	mci.request(mciAddress,request);
}

void Mote::confirm( MsgPayload& confirm )
{
//	if (confirm.result == SUCCESS) status = confirm.status;
	if (session)
	{
		session->confirm(mote_id,confirm);
	}	
}

bool Mote::setSession( Session* p_session )
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Query query = sqlConn.query();
	
	// check if the usage privilege has been granted
	query << "select priv_session_id from mote where id = %0:mote_id";
	query.parse();
	query.def["mote_id"] = mote_id;
	mysqlpp::ResUse res = query.use();
	res.disable_exceptions();
	mysqlpp::Row row = res.fetch_row();	
	if (!row || row.empty()) { __THROW__ ("Mote record not found in database!\n"); }

	if (row["priv_session_id"].is_null())
	{
		return false;
	}
	dbkey_t session_id = (dbkey_t) row["priv_session_id"];
	// free up the table again
	res.purge();
	
	if (session_id != p_session->session_id)
	{
		// no usage privilege granted
		return false;
	}
	
	if (session)
	{
		if ( session != p_session )
		{
			// tell the current client to drop its reference to this mote
			dropSession();
		}
		else
		{
			// session is already using this mote
			return false;
		}
	}	
	session = p_session;	
	
	// set usage info on the database mote record
	query.reset();
	query << "update mote set curr_session_id = %0:session_id where id = %1:mote_id";
	query.parse();
	query.def["session_id"] = p_session->session_id;
	query.def["mote_id"] = mote_id;
	query.execute();		
	return true;
	
}

void Mote::dropSession(bool notify)
{
	if ( session )
	{				
		// notify the client that it is being dropped
		if (notify)	session->freeMote(mote_id);


		mysqlpp::Connection& sqlConn = dbConn.getConnection();
		mysqlpp::Query moteupdate = sqlConn.query();
		// remove session usage privilege on the mote, if still present
		moteupdate << "update mote set priv_session_id=NULL where id=%0:mote_id and priv_session_id=%1:session_id";
		moteupdate.parse();
		moteupdate.def["mote_id"] = mote_id;
		moteupdate.def["session_id"] = session->session_id;
		moteupdate.execute();
		// reset usage info on the database mote record
		moteupdate.reset();
		moteupdate << "update mote set curr_session_id=NULL where id=%0:mote_id";
		moteupdate.parse();
		moteupdate.def["mote_id"] = mote_id;
		moteupdate.execute();
		session = NULL;
	}
}

void Mote::setAttribute(std::string type, std::string value)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Query query = sqlConn.query();

	// get attribute type id
	query << "select id from moteattrtype where name = '%0:type'";
	query.parse();
	query.def["type"] = type;
	mysqlpp::ResUse res = query.use();
	res.disable_exceptions();
	mysqlpp::Row row = res.fetch_row();	
	if (!row || row.empty()) { __THROW__ ("Mote attribute type not found!"); }
	dbkey_t type_id = (dbkey_t) row["id"];
	res.purge();
	query.reset();
	
	// remove old attribute if it exists
	query << "delete from mote_moteattr where mote_id=%0:mote_id and moteattr_id in (select id from moteattr where moteattrtype_id=%1:type_id)";
	query.parse();	
	query.def["mote_id"] = mote_id;
	query.def["type_id"] = type_id;
	query.execute();
	query.reset();
	
	// check if attribute value exists
	query << "select id from moteattr where moteattrtype_id=%0:type_id and val='%1:value'";
	query.parse();	
	query.def["type_id"] = type_id;
	query.def["value"] = value;
	res = query.use();
	res.disable_exceptions();
	row = res.fetch_row();
	dbkey_t attr_id;
	if (!row || row.empty()) {
		// create attribute value
		res.purge();
		query.reset();
		query << "insert into moteattr(moteattrtype_id,val) values(%0:type_id,'%1:value')";
		query.parse();
		query.def["type_id"] = type_id;
		query.def["value"] = value;
		mysqlpp::ResNSel sqlres = query.execute();	
		if (sqlres.success)	{
			attr_id = sqlres.insert_id;
		} else {
			__THROW__("Unable to create new mote attribute!");
		}		
	} else	{
		attr_id = (dbkey_t) row["id"];
		res.purge();
	}
	// assign attribute value
	query.reset();
	query << "insert into mote_moteattr(mote_id,moteattr_id) values(%0:mote_id,%1:attr_id)";
	query.parse();
	query.def["mote_id"] = mote_id;
	query.def["attr_id"] = attr_id;
	query.execute();	
}


// STATIC METHODS BELOW

result_t Mote::getById( dbkey_t p_mote_id,
                        Session* p_client,
                        Mote** p_mote )
{
	motemapbykey_t::iterator mi;
	Mote* themote;
	*p_mote = NULL;
		
	mi = mote.find(p_mote_id);

	
	if (mi != mote.end())
	{
		themote = mi->second;		
		
		if ( themote->setSession(p_client) )
		{
			*p_mote = themote;
			return SUCCESS;
		}
		else
		{
			return MOTE_OCCUPIED;
		}
	}
	else
	{		
		return MOTE_NOT_FOUND;
	}
}

void Mote::registerMote(Mote* newmote)
{
	motemapbykey_t::iterator mi;
	Mote* oldmote;	
	mi = mote.find(newmote->mote_id);
	if (mi != mote.end())
	{
		oldmote = mi->second;
		delete oldmote;
	}
	
	mote[newmote->mote_id] = newmote;
}	

void Mote::resetDb()
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	mysqlpp::Query reset = sqlConn.query();
	reset << "update mote set site_id = null";
	reset.parse();
	reset.execute();
}	

motemapbykey_t Mote::mote;

}}