#include "common/format.h"
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
	
	mysqlpp::Query pathinsert = sqlConn.query();	
	
	siteselect << "select site_id from path \
	               where host_id = %0:hostid \
	               and path='%1:path'";
	               
	pathinsert << "insert into path(host_id,path) \
	               values( %0:hostid,'%1:path' )";
	
	siteselect.parse();
	pathinsert.parse();	
	
	// look for the connection path + host id to get the site_id
	siteselect.def["hostid"] = id;
	siteselect.def["path"] = info.getPath().getString();
	
	selectRes = siteselect.use();
	selectRes.disable_exceptions();
	row = selectRes.fetch_row();
	if ( !row || row.empty() )
	{
		std::cout << "host_id " << id << " path " << info.getPath().getString() << "\n";
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
	moteselect << "select mote_id from moteattr ma, mote_moteattr mma, moteattrtype mat"
		      " where ma.val=" << mysqlpp::quote << getMacStr(info.macAddress)
		   << "   and mma.moteattr_id=ma.id"
		      "   and ma.moteattrtype_id=mat.id"
		      "   and mat.name='macaddress'";

	selectRes.purge();
	selectRes = moteselect.use();
	selectRes.disable_exceptions();
	row = selectRes.fetch_row();
	
	MoteAddresses* newtarget  = new MoteAddresses(0,info.macAddress);
	
	if ( !row || row.empty() )
	{
		std::string mac, tos;

		selectRes.purge();
		// create the mote using site_id only - the mote class will create the 
		// mote database record itself
		mote = new Mote(site_id,(MoteControlInfrastructure&)*this,*newtarget);
		// TODO: error checking
		// create the mac and tos address database records using the mote id
		newtarget->tosAddress = (uint16_t) mote->mote_id;

		mac = getMacStr(info.macAddress);
		tos = getTosStr(newtarget->tosAddress);
		log("Setting attributes: MAC=%s TOS=%s\n", mac.c_str(), tos.c_str());
		mote->setAttribute("macaddress", mac);
		mote->setAttribute("tosaddress", tos);
	}
	else
	{
		mote_id = (dbkey_t) row["mote_id"];   // save the mote_id		
		selectRes.purge();
		// create the mote object using mote_id and site_id - the mote class will
		// update the mote database record to reflect the new site
		mote = new Mote(mote_id,site_id,(MoteControlInfrastructure&)*this,*newtarget);

		// get the tos address mote attribute
		newtarget->tosAddress = atoi(mote->getAttribute("tosaddress").c_str());
		// TODO: error checking
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

		log("Mote %s plugged at %s\n", getMacStr(info.macAddress), info.getPath().getString().c_str());
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
