#include "MoteHost.h"

namespace remote { namespace diku_mch {

using namespace remote::util;

int MoteHost::clientsock;
int MoteHost::plugpipe;
Message MoteHost::msg;
DeviceManager MoteHost::devices;
Configuration MoteHost::config;

void MoteHost::lookForServer()
{
	while (1) {
		std::string host = config.vm["serverAddress"].as<std::string>();
		uint16_t port = config.vm["serverPort"].as<uint16_t>();
		uint64_t secs = config.vm["serverConnectionRetryInterval"].as<uint64_t>(); 

		Log::info("Connecting to %s on port %u...", host.c_str(), port);
		clientsock = openClientSocket(host, port);
		if (clientsock >= 0) {
			Log::info("Connected!");
			setKeepAlive(clientsock, 3, 120, 30);

			try {
				serviceLoop();
				close(clientsock);

			} catch (remote::protocols::MMSException e) {
				Log::error("Exception: %s", e.what());

			} catch (remote::error e) {
				Log::error("Exception: %s", e.what());
			}
			Log::error("Disconnected");

		} else {
			Log::error("Connection failed");
		}

		Log::info("Reconnecting in %llu seconds...", secs);
		usleep(secs * 1000000);
	}
}

void MoteHost::serviceLoop()
{
	std::string eventPipe = config.vm["usbPlugEventPipe"].as<std::string>();
	fd_set fdset;

	remove(eventPipe.c_str());
	if (mkfifo(eventPipe.c_str(), 0666) == -1)
		throw remote::error(errno, "Failed to make fifo " + eventPipe);

	plugpipe = open(eventPipe.c_str(), O_RDONLY | O_NONBLOCK);
	if (plugpipe < 0)
		throw remote::error(errno, "Failed to open fifo " + eventPipe);

	// the first thing to do is send all current mote information to the server
	devices.refresh(config.vm["devicePath"].as<std::string>());
	Log::debug("Sending mote list to server");
	MsgPlugEvent msgPlugEvent(PLUG_MOTES);
	if (makeMoteInfoList(devices.motes, msgPlugEvent.getInfoList())) {
		HostMsg hostMsg(msgPlugEvent);
		Message msg;
		msg.sendMsg(clientsock,hostMsg);
	}

	Log::debug("Entering service loop");
	while (true) {
		int maxfd = rebuildFdSet(fdset);

		// wait for non-blocking reads on the fds
		if (select(maxfd+1, &fdset, NULL, NULL, NULL) == -1)
			break;

		if (FD_ISSET(clientsock, &fdset))
			handleMessage();

		if (FD_ISSET(plugpipe, &fdset))
			handlePlugEvent();

		motemap_t::const_iterator moteI = devices.motes.begin();

		while (moteI != devices.motes.end()) {
			int p = moteI->second->getFd();
			if (p > 0 && FD_ISSET(p, &fdset))
				handleMoteData(moteI->second);
			moteI++;
		}
	}
	close(plugpipe);
}

int MoteHost::rebuildFdSet(fd_set& fdset)
{
	int p,maxp;
	Mote* pmote;
	FD_ZERO(&fdset);
	motemap_t::const_iterator moteI = devices.motes.begin();

	// put in valid mote file descriptors
	maxp = 0;
	while (moteI != devices.motes.end())
	{
		pmote = moteI->second;
		p = pmote->getFd();
		if (p > 0)
		{
			if (p > maxp) {maxp = p;}
			FD_SET(p, &fdset);
		}
		moteI++;
	}

	// fd for the connected server
	FD_SET(clientsock, &fdset);
	if (clientsock > maxp) {maxp = clientsock;}
	FD_SET(plugpipe, &fdset);
	if (plugpipe > maxp) {maxp = plugpipe;}
	return maxp;
}

void MoteHost::handlePlugEvent()
{
	Message msg;
	// for now, just assume that a plug event occured
	char c;
	// empty the pipe
	int i = 1;
	Log::info("Handling plug event");
	while ( i == 1 )
	{
		i = read(plugpipe,&c,1);
		if ( i <= 0 )
		{
			close(plugpipe);
			plugpipe = open(config.vm["usbPlugEventPipe"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
		}
	}

	devices.refresh(config.vm["devicePath"].as<std::string>());

	MsgPlugEvent msgUnplugEvent(UNPLUG_MOTES);
	if (makeMoteInfoList(devices.lostMotes, msgUnplugEvent.getInfoList()))
	{
		HostMsg hostMsg(msgUnplugEvent);
		msg.sendMsg(clientsock,hostMsg);
	}

	MsgPlugEvent msgPlugEvent(PLUG_MOTES);
	if (makeMoteInfoList(devices.newMotes,msgPlugEvent.getInfoList()))
	{
		HostMsg hostMsg(msgPlugEvent);
		msg.sendMsg(clientsock,hostMsg);
	}
}

bool MoteHost::makeMoteInfoList(motemap_t& motelist, MsgMoteConnectionInfoList& infolist)
{
	motemap_t::const_iterator moteI;

	infolist.clear();

	for (moteI = motelist.begin(); moteI != motelist.end(); moteI++) {
		Mote *mote = moteI->second;
		MsgMoteConnectionInfo info(mote->getMac(), mote->getDevicePath(),
					   mote->getPlatform());

		infolist.addMoteInfo(info);
	}

	return motelist.size() > 0 ? true : false;
}


void MoteHost::handleMessage()
{
	motemap_t::const_iterator moteI;
	Mote *mote;

	if (msg.nonBlockingRecv(clientsock))
	{
		uint8_t* buffer = msg.getData();
		uint32_t buflen = msg.getLength();
		HostMsg hostMsg(buffer,buflen);

		if (hostMsg.getType() != HOSTMSGTYPE_HOSTREQUEST)
			throw remote::error("Can only handle hostmote messages!");

		MsgHostRequest& msgHostRequest = hostMsg.getHostRequest();
		MsgMoteAddresses& addresses = msgHostRequest.getMoteAddresses();
		buffer = (uint8_t*)msgHostRequest.getMessage().getData();
		buflen = msgHostRequest.getMessage().getDataLength();
		MoteMsg moteMsg(buffer,buflen);

		moteI = devices.motes.find(addresses.getMac());

		if (moteI == devices.motes.end())
		{
			Log::warn("Mote %s unknown!", addresses.getMac().c_str());
			MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_UNKNOWN_MOTE,addresses,msgHostRequest.getMessage());
			HostMsg msgReply(msgHostConfirm);
			msg.sendMsg(clientsock,msgReply);
			return;
		}

		mote = moteI->second;

		switch (moteMsg.getType()) {
		case MOTEMSGTYPE_REQUEST:
			handleRequest(mote,addresses,moteMsg.getRequest());
			break;
		case MOTEMSGTYPE_DATA:
		{
			MsgPayload payload = moteMsg.getData();
			mote->writeBuf((const char*)payload.getData(),payload.getDataLength());
			break;
		}
		default:
			throw remote::error("Invalid message type!");
		}
	}
}

void MoteHost::handleRequest(Mote* mote, MsgMoteAddresses& addresses, MsgRequest& request)
{
	uint8_t command = request.getCommand();
	result_t result;

	Log::debug("Mote %s got command=%u", addresses.getMac().c_str(), command);

	switch (command)
	{
		case MOTECOMMAND_PROGRAM:
			result = mote->program(addresses.getTosAddress(),
					       request.getFlashImage().getData(),
					       request.getFlashImage().getDataLength());
			// don't confirm until programming is done
			if (result == SUCCESS)
				return;
			break;
		case MOTECOMMAND_CANCELPROGRAMMING:
			Log::info("User cancelling programming");
			result = mote->cancelProgramming();
			break;
		case MOTECOMMAND_STATUS:
			result = SUCCESS;
			break;
		case MOTECOMMAND_RESET:
			result = mote->reset();
			break;
		case MOTECOMMAND_START:
			result = mote->start();
			break;
		case MOTECOMMAND_STOP:
			result = mote->stop();
			break;
		default:
			Log::error("Unkown command %u", command);
			return;
	}

	confirmRequest(mote, command, result);
}

void MoteHost::handleMoteData(Mote* mote)
{
	char buf[1000];
	ssize_t readlen = sizeof(buf);

	MsgMoteAddresses msgMoteAddresses(mote->getMac());

	while (readlen == sizeof(buf)) {
		readlen = mote->readBuf(buf, sizeof(buf));
		if (readlen > 0) {
			Log::debug("'%.*s'", readlen, buf);
			uint32_t len = readlen;
			MsgPayload msgData;
			msgData.setPayload(len,(uint8_t*)buf);
			MoteMsg moteMsg(msgData);
			MsgPayload msgPayload(moteMsg);
			MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,msgMoteAddresses,msgPayload);
			HostMsg hostMsg(msgHostConfirm);
			Message msg;
			msg.sendMsg(clientsock,hostMsg);
		}
	}

	// check if we're done programming
	if (readlen <= 0) {
		if (mote->getStatus() == MOTE_PROGRAMMING) {
			Log::info("Programming done!");
			remove(mote->getImagePath().c_str());
			result_t result = mote->getChildResult();
			confirmRequest(mote, MOTECOMMAND_PROGRAM, result);
		}
	}
}

void MoteHost::confirmRequest(Mote *mote, uint8_t command, result_t result)
{
	MsgMoteAddresses addresses(mote->getMac());
	MsgConfirm msgConfirm(command, result, mote->getStatus());
	MoteMsg moteMsg(msgConfirm);
	MsgPayload msgPayload(moteMsg);
	MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,addresses,msgPayload);
	HostMsg hostMsg(msgHostConfirm);
	Message msg;
	msg.sendMsg(clientsock, hostMsg);
}

int MoteHost::main(int argc,char** argv)
{
	config.read(argc,argv);
	Log::open("diku_mch", LOG_INFO);
	if (config.vm["daemonize"].as<int>()) {
		switch (fork()) {
		case -1:
			Log::error("Failed to fork daemon");
			exit(EXIT_FAILURE);

		default:
			_exit(EXIT_SUCCESS);

		case 0:
			Log::info("Running as daemon");
		}

		setsid();
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
	}

	MoteHost::lookForServer();
	return 0;
}

}}

int main(int argc,char** argv)
{
	return remote::diku_mch::MoteHost::main(argc,argv);
}
