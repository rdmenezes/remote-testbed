#include "MoteHost.h"

namespace remote { namespace diku_mch {

int MoteHost::clientsock;
int MoteHost::plugpipe;
fd_set MoteHost::fdset;
Message MoteHost::msg;
DeviceManager MoteHost::devices;
Configuration MoteHost::config;

void MoteHost::lookForServer()
{
	while (1)
	{
		printf("Attempting to connect to server %s using port %u...\n", config.vm["serverAddress"].as<std::string>().c_str(),config.vm["serverPort"].as<uint16_t>());

		clientsock = openClientSocket(config.vm["serverAddress"].as<std::string>(), config.vm["serverPort"].as<uint16_t>() );
		// set keepalive options
		setKeepAlive( clientsock, 3, 120, 30);

		if (clientsock >= 0)
		{
			printf("Server connected...\n");
			try
			{
				serviceLoop();
				close(clientsock);
			}
			catch (remote::protocols::MMSException e)
			{
				fprintf(stderr,"Exception: %s\n",e.what());
			}
			printf( "Server disconnected, attempting reconnect in %llu seconds\n",config.vm["serverConnectionRetryInterval"].as<uint64_t>());
			usleep(config.vm["serverConnectionRetryInterval"].as<uint64_t>()*1000000);
		}
		else
		{
			printf("Server connection failed, trying again in %llu seconds...\n",config.vm["serverConnectionRetryInterval"].as<uint64_t>());
			usleep(config.vm["serverConnectionRetryInterval"].as<uint64_t>()*1000000);
		}
	}
}

void MoteHost::serviceLoop()
{
	int res = 0 ,p;

	remove(config.vm["usbPlugEventPipe"].as<std::string>().c_str());
	if ( mkfifo(config.vm["usbPlugEventPipe"].as<std::string>().c_str(),666) == -1)
	{
		std::string err = "Failed to make fifo "+config.vm["usbPlugEventPipe"].as<std::string>()+": "+strerror(errno);
		__THROW__ (err.c_str());
	}
	plugpipe = open(config.vm["usbPlugEventPipe"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
	if ( plugpipe < 0 )
	{
		std::string err = "Failed to open fifo "+config.vm["usbPlugEventPipe"].as<std::string>()+": "+strerror(errno);
		__THROW__ (err.c_str());
	}
	// the first thing to do is send all current mote information to the server
	devices.refresh(config.vm["devicePath"].as<std::string>());
	printf("Sending mote list to server\n");
	MsgPlugEvent msgPlugEvent(PLUG_MOTES);
	if (makeMoteInfoList(devices.motes, msgPlugEvent.getInfoList()))
	{
		HostMsg hostMsg(msgPlugEvent);
		Message msg;
		msg.sendMsg(clientsock,hostMsg);
	}

	printf("Building initial fd set.\n");
	// prepare file descriptors
	int maxfd = rebuildFdSet(fdset);
	printf("Entering service loop.\n");
	while ( res > -1 )
	{
		// wait for non-blocking reads on the fds
		res = select(maxfd+1, &fdset, NULL, NULL, NULL);

		if ( res > -1 )
		{
			if (FD_ISSET(clientsock,&fdset))
			{
				handleMessage();
			}

			if (FD_ISSET(plugpipe,&fdset))
			{
				handlePlugEvent();
			}

			motemap_t::const_iterator moteI = devices.motes.begin();

			while (moteI != devices.motes.end())
			{
				p = moteI->second->getFd();
				if (p > 0 && FD_ISSET(p,&fdset))
				{
					handleMoteData(moteI->second);
				}
				moteI++;
			}
		}

		maxfd = rebuildFdSet(fdset);
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
		if (p > 0 && pmote->isValid())
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
	printf("\nHandling plug event\n");
	while ( i == 1 )
	{
		i = read(plugpipe,&c,1);
		if ( i <= 0 )
		{
			close(plugpipe);
			plugpipe = open(config.vm["usbPlugEventPipe"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
		}
		else
		{
			printf("%c",c);
		}
	}
	printf("\n");

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
		MsgMoteConnectionInfo info(mote->getMac(), mote->getDevicePath());

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
		{
			__THROW__ ("Can only handle hostmote messages!");
		}
		MsgHostRequest& msgHostRequest = hostMsg.getHostRequest();
		MsgMoteAddresses& addresses = msgHostRequest.getMoteAddresses();
		buffer = (uint8_t*)msgHostRequest.getMessage().getData();
		buflen = msgHostRequest.getMessage().getDataLength();
		MoteMsg moteMsg(buffer,buflen);

		moteI = devices.motes.find(addresses.getMac());

		if (moteI == devices.motes.end())
		{
			printf("Mote %s unknown!\n", addresses.getMac().c_str());
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
			__THROW__ ("Invalid message type!");
		}
	}
}

void MoteHost::handleRequest(Mote* mote, MsgMoteAddresses& addresses, MsgRequest& request)
{
	uint8_t command = request.getCommand();
	result_t result;

	printf("Mote %s got command=%u\n", addresses.getMac().c_str(), command);

	switch (command)
	{
		case MOTECOMMAND_PROGRAM:
			result = program(mote, addresses, request.getFlashImage());
			// don't confirm until programming is done
			if (result == SUCCESS)
				return;
			break;
		case MOTECOMMAND_CANCELPROGRAMMING:
			printf("User cancelling programming\n");
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
			printf("Unknown command %u\n", command);
			return;
	}

	{
		MsgConfirm msgConfirm(command, result, mote->getStatus());
		MoteMsg moteMsg(msgConfirm);
		MsgPayload msgPayload(moteMsg);
		MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,addresses,msgPayload);
		HostMsg hostMsg(msgHostConfirm);
		Message msg;
		msg.sendMsg(clientsock,hostMsg);
	}
}

void MoteHost::handleMoteData(Mote* mote)
{
	char buf[1000];
	ssize_t readlen = sizeof(buf);

	MsgMoteAddresses msgMoteAddresses(mote->getMac());

	while (readlen == sizeof(buf)) {
		readlen = mote->readBuf(buf, sizeof(buf));
		if (readlen > 0) {
			printf("'%.*s'", readlen, buf);
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
	if ( readlen <= 0 )
	{
		result_t result = mote->getChildResult();

		if (result != NOT_SUPPORTED) {
			printf("Programming done!\n");
			remove(mote->getImagePath().c_str());
			MsgConfirm msgConfirm(MOTECOMMAND_PROGRAM, result, mote->getStatus());
			MoteMsg moteMsg(msgConfirm);
			MsgPayload msgPayload(moteMsg);
			MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,msgMoteAddresses,msgPayload);
			HostMsg hostMsg(msgHostConfirm);
			Message msg;
			msg.sendMsg(clientsock,hostMsg);
		} else {
			mote->invalidate();
			mote->closeTty();
			log("Invalidating mote '%s'\n", mote->getMac().c_str());
		}
	}
}

bool MoteHost::writeImageFile(std::string filename, MsgPayload& image)
{
	ssize_t filesize;
	int fd = open(filename.c_str(), O_CREAT | O_TRUNC | O_WRONLY);

	if (fd < 0)
		return false;

	filesize = write(fd, (const void *) image.getData(), image.getDataLength());
	close(fd);
	if (filesize == (ssize_t) image.getDataLength())
		return true;

	remove(filename.c_str());
	return false;
}

result_t MoteHost::program(Mote *mote, MsgMoteAddresses& addresses, MsgPayload& image)
{
	std::string filename = mote->getImagePath();

	if (mote->getStatus() == MOTE_PROGRAMMING)
		return FAILURE;

	if (writeImageFile(filename, image)) {
		std::string mac_env = "macaddress=" + mote->getMac();
		std::string tos_env = "tosaddress=" + addresses.getTosAddress();
		char * const args[] = {
			(char *) mote->getProgrammerPath().c_str(),
			(char *) mote->getTty().c_str(),
			(char *) filename.c_str(),
			NULL
		};
		char * const envp[] = {
			(char *) mac_env.c_str(),
			(char *) tos_env.c_str(),
			NULL
		};

		log("Programming TTY %s\n", mote->getTty().c_str());

		if (mote->runChild(args, envp))
			return SUCCESS;

		remove(filename.c_str());
	}

	return FAILURE;
}


int MoteHost::main(int argc,char** argv)
{
	config.read(argc,argv);
	if (config.vm["daemonize"].as<int>())
	{
		printf("Daemonizing!\n");
		if (fork()) exit(0);
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
