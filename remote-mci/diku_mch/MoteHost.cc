#include "MoteHost.h"

namespace remote { namespace diku_mch {

int MoteHost::clientsock;
int MoteHost::plugpipe;
fd_set MoteHost::fdset;
Message MoteHost::msg;

void MoteHost::lookForServer()
{
	while (1)
	{
		printf("Attempting to connect to server %s using port %u...\n", Configuration::vm["serverAddress"].as<std::string>().c_str(),Configuration::vm["serverPort"].as<uint16_t>());

		clientsock = openClientSocket(Configuration::vm["serverAddress"].as<std::string>(), Configuration::vm["serverPort"].as<uint16_t>() );
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
			printf( "Server disconnected, attempting reconnect in %llu seconds\n",Configuration::vm["serverConnectionRetryInterval"].as<uint64_t>());
			usleep(Configuration::vm["serverConnectionRetryInterval"].as<uint64_t>()*1000000);
		}
		else
		{
			printf("Server connection failed, trying again in %llu seconds...\n",Configuration::vm["serverConnectionRetryInterval"].as<uint64_t>());
			usleep(Configuration::vm["serverConnectionRetryInterval"].as<uint64_t>()*1000000);
		}
	}
}

void MoteHost::serviceLoop()
{
	int res = 0 ,p;

	remove(Configuration::vm["usbPlugEventPipe"].as<std::string>().c_str());
	if ( mkfifo(Configuration::vm["usbPlugEventPipe"].as<std::string>().c_str(),666) == -1)
	{
		std::string err = "Failed to make fifo "+Configuration::vm["usbPlugEventPipe"].as<std::string>()+": "+strerror(errno);
		__THROW__ (err.c_str());
	}
	plugpipe = open(Configuration::vm["usbPlugEventPipe"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
	if ( plugpipe < 0 )
	{
		std::string err = "Failed to open fifo "+Configuration::vm["usbPlugEventPipe"].as<std::string>()+": "+strerror(errno);
		__THROW__ (err.c_str());
	}
	// the first thing to do is send all current mote information to the server
	DeviceManager::refresh();
	printf("Sending mote list to server\n");
	MsgPlugEvent msgPlugEvent(PLUG_MOTES);
	if (makeMoteInfoList(DeviceManager::motes,msgPlugEvent.getInfoList()))
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

			motemap_t::const_iterator moteI = DeviceManager::motes.begin();

			while (moteI != DeviceManager::motes.end())
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
	motemap_t::const_iterator moteI = DeviceManager::motes.begin();

	// put in valid mote file descriptors
	maxp = 0;
	while (moteI != DeviceManager::motes.end())
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
	printf("\nHandling plug event\n");
	while ( i == 1 )
	{
		i = read(plugpipe,&c,1);
		if ( i <= 0 )
		{
			close(plugpipe);
			plugpipe = open(Configuration::vm["usbPlugEventPipe"].as<std::string>().c_str(),O_RDONLY | O_NONBLOCK);
		}
		else
		{
			printf("%c",c);
		}
	}
	printf("\n");

	DeviceManager::refresh();

	MsgPlugEvent msgUnplugEvent(UNPLUG_MOTES);
	if (makeMoteInfoList(DeviceManager::lostMotes,msgUnplugEvent.getInfoList()))
	{
		HostMsg hostMsg(msgUnplugEvent);
		msg.sendMsg(clientsock,hostMsg);
	}

	MsgPlugEvent msgPlugEvent(PLUG_MOTES);
	if (makeMoteInfoList(DeviceManager::newMotes,msgPlugEvent.getInfoList()))
	{
		HostMsg hostMsg(msgPlugEvent);
		msg.sendMsg(clientsock,hostMsg);
	}
}

bool MoteHost::makeMoteInfoList(motemap_t& motelist, MsgMoteConnectionInfoList& infolist)
{

	Mote* pmote;
	motemap_t::const_iterator moteI;
	MsgMoteConnectionInfo info;

	infolist.clear();

	if ( motelist.size() > 0 )
	{
		for ( moteI = motelist.begin() ; moteI != motelist.end() ; moteI++ )
		{
			pmote = moteI->second;
			uint64_t mac = pmote->getMac();
			printf("Mote %x at %s\n",((uint32_t)mac),pmote->getPath().c_str());
			info.macAddress = mac;
			info.getPath() = pmote->getPath();
			infolist.addMoteInfo(info);
		}
		return true;
	}
	else
	{
		return false;
	}
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

		moteI = DeviceManager::motes.find(addresses.getMacAddress());
		printf("HOSTMSGTYPE_MOTEMSG for TOS %u",addresses.getTosAddress());
		printf(" MAC:%x%x\n",
		       (unsigned int) (addresses.getMacAddress() >> 32),
		       (unsigned int) addresses.getMacAddress());

		if (moteI == DeviceManager::motes.end())
		{
			printf("UNKNOWN MOTE!");
			MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_UNKNOWN_MOTE,addresses,msgHostRequest.getMessage());
			HostMsg msgReply(msgHostConfirm);
			msg.sendMsg(clientsock,msgReply);
			return;
		}

	mote = moteI->second;

	switch (moteMsg.getType())
	{
		case MOTEMSGTYPE_REQUEST:
			handleRequest(mote,addresses,moteMsg.getRequest());
			break;
		case MOTEMSGTYPE_DATA:
			{
				MsgPayload payload = moteMsg.getData();
				mote->writeBuf((const char*)payload.getData(),payload.getDataLength());
			}
			break;
		default:
			__THROW__ ("Invalid message type!");
	}
	}
}

void MoteHost::handleRequest(Mote* mote,MsgMoteAddresses& addresses, MsgRequest& request)
{
	bool reply = true;
	uint8_t command = request.getCommand();
	uint8_t result,status;

	switch (command)
	{
		case MOTECOMMAND_PROGRAM:
			if ( program(mote,addresses.getTosAddress(),request.getFlashImage()))
			{
				// don't confirm until programming is done
				reply = false;
				return;
			}
			else
			{
				result = FAILURE;
				status = mote->getStatus();
			}
			break;
		case MOTECOMMAND_CANCELPROGRAMMING:
			printf("User cancelling programming\n");
			result = mote->cancelProgramming();
			status = mote->getStatus();
			break;
		case MOTECOMMAND_STATUS:
			result = SUCCESS;
			status = mote->getStatus();
			break;
		case MOTECOMMAND_RESET:
			result = mote->reset();
			status = mote->getStatus();
			break;
		case MOTECOMMAND_START:
			result = mote->start();
			status = mote->getStatus();
			break;
		case MOTECOMMAND_STOP:
			result = mote->stop();
			status = mote->getStatus();
			break;
		default:
			printf("Unkown command %u\n",command);
			return;
	}

	if (reply)
	{
		MsgConfirm msgConfirm(command,result,status);
		MoteMsg moteMsg(msgConfirm);
		MsgPayload msgPayload(moteMsg);
		MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,addresses,msgPayload);
		HostMsg hostMsg(msgHostConfirm);
		Message msg;
		msg.sendMsg(clientsock,hostMsg);
	}
}

void MoteHost::handleMoteData(Mote* p_mote)
{

	int readlen = 1000;
	char* buf = new char[1000];

	MsgMoteAddresses msgMoteAddresses(0,p_mote->getMac());

	while ( readlen == 1000 )
	{
		readlen = p_mote->readBuf(buf,1000);
		if (readlen > 0)
		{
			printf("'%s'",std::string(buf,readlen).c_str());
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
		uint8_t result;
		if (p_mote->getProgrammingResult(result))
		{
			printf("Programming done!\n");
			MsgConfirm msgConfirm(MOTECOMMAND_PROGRAM,result,p_mote->getStatus());
			MoteMsg moteMsg(msgConfirm);
			MsgPayload msgPayload(moteMsg);
			MsgHostConfirm msgHostConfirm(MSGHOSTCONFIRM_OK,msgMoteAddresses,msgPayload);
			HostMsg hostMsg(msgHostConfirm);
			Message msg;
			msg.sendMsg(clientsock,hostMsg);
		}
	}

	delete buf;
}

bool MoteHost::program(Mote* p_mote, uint16_t tosAddress, MsgPayload& image)
{
	int fd;
	std::string filename;
	char nstring[100];

	if ( p_mote->getStatus() == MOTE_PROGRAMMING )
	{
		return false;
	}
	sprintf(nstring,"/var/run/motehostprg%u.s19",p_mote->getTty());
	filename = nstring;
	// create a file
	fd = open(filename.c_str(),O_CREAT | O_TRUNC | O_WRONLY);
	if (fd > 0)
	{
		// write the entire image to the file
		if (write(fd,(const void*)image.getData(),image.getDataLength()) != (ssize_t) image.getDataLength())
		{
			close(fd);
			remove(filename.c_str());
			return false;
		}

		// program the mote
		p_mote->program(p_mote->getMac(),tosAddress,filename);
		return true;
	}
	return false;
}


int MoteHost::main(int argc,char** argv)
{
	Configuration::read(argc,argv);
	if (Configuration::vm["daemonize"].as<int>())
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
	return  remote::diku_mch::MoteHost::main(argc,argv);
}