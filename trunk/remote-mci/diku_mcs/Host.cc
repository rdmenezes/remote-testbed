#include "Host.h"
#include "macros.h"

namespace remote { namespace diku_mcs {

Host::Host( int fd, dbkey_t p_id, std::string ip, hostmapbykey_t& p_hostMap )
     : FileDescriptor(fd), id(p_id), message_in(), ipaddress(ip), hostMap(p_hostMap)
{
	hostMap[id] = this;
	protocols::setSendTimeout( fd,5,0); // 5 second host send timeout should be adequate
	// set keepalive options: 3 attempts after 120 seconds, 30 second interval
	setKeepAlive( fd, 3, 120, 30);
}

Host::~Host()
{
	log("Closing host %s!\n",ipaddress.c_str());
	hostMap.erase(id);
}

void Host::destroy(bool silent)
{
	log("Deleting motes\n");
	motemapbymac_t::iterator mI;	
	for( mI = motesByMac.begin(); mI != motesByMac.end(); mI++)
	{
		if (mI->second)	mI->second->destroy(silent);
	}	
	delete this;
}

void Host::registerMoteByMac(uint64_t mac,Mote* mote)
{
	motemapbymac_t::iterator mI;
	
	mI = motesByMac.find(mac);
	
	if ( mI != motesByMac.end() )
	{
		mI->second->destroy();
	}

	motesByMac[mac] = mote;
}

void Host::deleteMoteByMac(uint64_t mac)
{
	motemapbymac_t::iterator mI;
	Mote* mote = NULL;
	mI = motesByMac.find(mac);
		
	if ( mI != motesByMac.end() )
	{
		mote = mI->second;
		motesByMac.erase(mI);
	}
		
	if (mote)
	{
		mote->destroy();
	}
}

void Host::registerMoteInfoByMac(uint64_t mac, moteinfo_t moteinfo)
{
	moteInfoByMac[mac] = moteinfo;
}

void Host::deleteMoteInfoByMac(uint64_t mac)
{
	moteinfobymac_t::iterator mI;
	mI = moteInfoByMac.find(mac);		
	if ( mI != moteInfoByMac.end() )
	{
		moteInfoByMac.erase(mI);
	}
}

void Host::findOrCreateMote(MsgMoteConnectionInfo& info)
{
	mysqlpp::Connection& sqlConn = dbConn.getConnection();
	dbkey_t site_id, mote_id;
	Mote* mote;
	mysqlpp::ResUse selectRes;
	mysqlpp::ResNSel execRes;
	mysqlpp::Row row;

	mysqlpp::Query siteselect = sqlConn.query();	
	mysqlpp::Query moteselect = sqlConn.query();
	mysqlpp::Query tosselect = sqlConn.query();	
	
	mysqlpp::Query pathinsert = sqlConn.query();	
	mysqlpp::Query macinsert = sqlConn.query();	
	mysqlpp::Query tosinsert = sqlConn.query();
	
	siteselect << "select site_id from path \
	               where host_id = %0:hostid \
	               and path='%1:path'";
	               
	moteselect << "select mote_id from macaddress \
	               where macaddress = %0:macaddress";
	               
	tosselect << "select tosaddress from tosaddress \
	              where mote_id = %0:mote_id";
	               
	pathinsert << "insert into path(host_id,path) \
	               values( %0:hostid,'%1:path' )";
	               
	macinsert << "insert into macaddress(macaddress,mote_id) \
	              values ( %0:macaddress,%1:mote_id)";
	
	tosinsert << "insert into tosaddress(mote_id) values( %0:mote_id )";
	
	moteselect.parse();
	siteselect.parse();
	tosselect.parse();
	
	pathinsert.parse();	
	macinsert.parse();
	tosinsert.parse();	

	
	// look for the connection path + host id to get the site_id
	siteselect.def["hostid"] = id;
	siteselect.def["path"] = info.getPath().getString();
	
	selectRes = siteselect.use();
	selectRes.disable_exceptions();
	row = selectRes.fetch_row();
	if ( !row || row.empty() )
	{
		// if not found, create the path in the database with no site_id
		pathinsert.def["hostid"] = id;
		pathinsert.def["path"] = info.getPath().getString();
		pathinsert.execute(); // TODO: error checking
		
		// get the newly inserted paths site_id
		selectRes.purge();
		selectRes = siteselect.use();
		selectRes.disable_exceptions();
		row = selectRes.fetch_row();
	}
	
	// save the site_id
	site_id = (dbkey_t) row["site_id"];
	
	// look for the mac addresses in the database, get mote_id	
	moteselect.def["macaddress"] = info.macAddress;

	selectRes.purge();
	selectRes = moteselect.use();
	selectRes.disable_exceptions();
	row = selectRes.fetch_row();
	
	MoteAddresses* newtarget  = new MoteAddresses(0,info.macAddress);
	
	if ( !row || row.empty() )
	{
		selectRes.purge();
		// create the mote using site_id only - the mote class will create the 
		// mote database record itself
		mote = new Mote(site_id,(MoteControlInfrastructure&)*this,*newtarget);
		// TODO: error checking
		// create the mac and tos address database records using the mote id
		macinsert.def["macaddress"] = info.macAddress;
		macinsert.def["mote_id"] = mote->mote_id;
		macinsert.execute();
		tosinsert.def["mote_id"] = mote->mote_id;
		execRes = tosinsert.execute();		
		newtarget->tosAddress = execRes.insert_id;
		// FIXME: convert MAC address to hex string!!
		//mote->setAttribute("macaddress",std::string::info.macAddress);
		// TODO: tos address as well
	}
	else
	{
		mote_id = (dbkey_t) row["mote_id"];   // save the mote_id		
		selectRes.purge();
		// create the mote object using mote_id and site_id - the mote class will
		// update the mote database record to reflect the new site
		mote = new Mote(mote_id,site_id,(MoteControlInfrastructure&)*this,*newtarget);

		// add tosAddress as external mote info
		tosselect.def["mote_id"] = mote_id; // get the tos address
		selectRes = tosselect.use();
		selectRes.disable_exceptions();
		row = selectRes.fetch_row();		
		newtarget->tosAddress = (uint16_t)row["tosaddress"];
	}
	
	if (mote)
	{
		// finally, register the new mote in the local map
		registerMoteByMac(info.macAddress,mote);
	}	
}

void Host::handleMotesLostList(MsgMoteConnectionInfoList& infolist)
{
	MsgMoteConnectionInfo info;	
	while ( infolist.getNextMoteInfo(info) )
	{
		deleteMoteByMac(info.macAddress);
		deleteMoteInfoByMac(info.macAddress);
	}
}

void Host::handleMotesFoundList(MsgMoteConnectionInfoList& infolist)
{
	MsgMoteConnectionInfo info;	

	while ( infolist.getNextMoteInfo(info) )
	{				
		uint8_t* m = (uint8_t*)&info.macAddress;
		log("Mote %02x%02x%02x%02x %02x%02x%02x%02x plugged at %s\n",m[7],m[6],m[5],m[4],m[3],m[2],m[1],m[0],info.getPath().getString().c_str());
		
		findOrCreateMote(info);
	}
}

void Host::request(MCIAddress& address, MsgPayload& request )
{
	MsgMoteAddresses addresses(((MoteAddresses&)address).tosAddress,
	                           ((MoteAddresses&)address).macAddress);
	MsgHostRequest msgHostRequest(addresses,request);
	HostMsg message(msgHostRequest);

	try {
		Message msg;
		msg.sendMsg(fd,message);
	}
	catch (remote::protocols::MMSException e)
	{
		log("Exception: %s\n",e.what());
		this->destroy(true);
	}
}


void Host::handleEvent(short events)
{
	
	if ( (events & POLLIN) || (events & POLLPRI) )
	{
		if (!message_in.nonBlockingRecv(fd))
		{
			 return;
		}
		
		uint32_t msglen = message_in.getLength();
		uint8_t* buffer = message_in.getData();
		HostMsg message(buffer,msglen);
		
		switch (message.getType())
		{
			case HOSTMSGTYPE_HOSTCONFIRM:
				handleMsgIn(message.getHostConfirm());
				break;
			case HOSTMSGTYPE_PLUGEVENT:
				if ( message.getPlugEvent().getType() == PLUG_MOTES )
				{
					handleMotesFoundList(message.getPlugEvent().getInfoList());
				}
				else if ( message.getPlugEvent().getType() == UNPLUG_MOTES )
				{
					handleMotesLostList(message.getPlugEvent().getInfoList());
				}
				else
				{
					__THROW__ ("Invalid plugevent received from host!");
				}
				break;
			default:
				__THROW__ ("Invalid message type received from host!");
				break;
		}
	}
	else if ( (events & POLLERR) || (events & POLLHUP) || (events & POLLNVAL) )
	{
		__THROW__ ("Host connection error!\n");
	}	
}

void Host::handleMsgIn(MsgHostConfirm& msgHostConfirm)
{
	motemapbymac_t::iterator mI;	
	mI = motesByMac.find(msgHostConfirm.getMoteAddresses().getMacAddress()); 
	if ( mI != motesByMac.end() )
	{
		if (msgHostConfirm.getStatus() == MSGHOSTCONFIRM_UNKNOWN_MOTE)
		{
			deleteMoteByMac(msgHostConfirm.getMoteAddresses().getMacAddress());
			deleteMoteInfoByMac(msgHostConfirm.getMoteAddresses().getMacAddress());
		} else {
			mI->second->confirm(msgHostConfirm.getMessage());
		}
	}
}
		
}}