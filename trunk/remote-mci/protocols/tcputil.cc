#include "tcputil.h"

namespace remote { namespace protocols {

int openServerSocket(struct sockaddr_in& server, unsigned int port, int max_pending, int retryinterval)
{
	int serversock = -1 ;

	do {
		/* Create the TCP socket */
		if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			Log::warn("Could not create server socket, waiting to try again");
			usleep(retryinterval * 1000000);
		}
	} while (serversock < 0);


	/* Construct the server sockaddr_in structure */
	memset(&server, 0, sizeof(server));		/* Clear struct */
	server.sin_family = AF_INET;				/* Internet/IP */
	server.sin_addr.s_addr = htonl(INADDR_ANY);	/* Incoming addr */
	server.sin_port = htons(port);			/* server port */
	/* Bind the server socket */
	while (bind(serversock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		Log::warn("Could not bind server socket, waiting to try again");
		usleep(retryinterval * 1000000);
	}

	/* Listen on the server socket */
	if (listen(serversock, max_pending) < 0) {
		Log::error("Could not listen on server socket.");
		return -1;
	}

	return serversock;
}

int nextClient(int serversock, sockaddr_in& client)
{
	int clientsock;
	unsigned int clientlen = sizeof(client);

	/* Wait for client connection */
	if ((clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
		Log::error("Could not accept client connection: %s", strerror(errno));
	else
		Log::info("Accepted connection from %s", inet_ntoa(client.sin_addr));
	return clientsock;
}

int openClientSocket(std::string address, unsigned int port)
{
	int sock;
	struct sockaddr_in server;

	// Create the TCP control socket
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Log::error("Failed to create client socket: %s", strerror(errno));
		return -1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = resolve(address.c_str());
	server.sin_port = htons(port);

	// Establish connection
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		Log::error("Failed to connect to %s on port %u: %s",
			   address.c_str(), port, strerror(errno));
		close(sock);
		return -1;
	}

	return sock;
}

in_addr_t resolve(const char *ip_addr)
{
	struct hostent *hp;
	in_addr_t ip;
	hp = gethostbyname(ip_addr);

	if (!hp)
	{
		ip = inet_addr(ip_addr);
		if (ip == INADDR_NONE) return INADDR_NONE;
		else return ip;
	}

	// hp->h_length should equal to 4
	memcpy(&ip, hp->h_addr, 4);
	return ip;
}


const char *getHostByIp(in_addr ip)
{
	char *ipaddr = inet_ntoa(ip);
	hostent* host = gethostbyaddr(ipaddr,strlen(ipaddr),AF_INET);

	if (host) {
		Log::info("Resolved %s to host name %s", ipaddr, host->h_name);
		return host->h_name;
	}

	Log::error("Failed to resolve %s: %s", ipaddr, hstrerror(h_errno));
	return "";
}

void setSendTimeout(int fd, long seconds, long microseconds )
{
	struct timeval timeOut;

	timeOut.tv_sec = seconds;
	timeOut.tv_usec = microseconds;
	if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeOut, sizeof(timeOut)) != 0) {
		Log::error("Failed to set SO_SNDTIMEO to %i: %s",
			   fd, strerror(errno));
	}
}

void setSendBuffer( int fd, int byteSize)
{
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &byteSize, sizeof(byteSize)) != 0) {
		Log::error("Failed to set SO_SNDBUF to %i on %i: %s",
			   byteSize, fd, strerror(errno));
	}
}

void setKeepAlive( int fd, int numProbes, int idleTime, int interval)
{
	// set keepalive on
	int optval = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval,sizeof(optval)) != 0) {
		Log::error("Failed to set SO_KEEPALIVE on %i: %s", fd, strerror(errno));
		return;
	}

	// The maximum number of keepalive probes TCP should send before dropping the connection.
	if (setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &numProbes, sizeof(numProbes)) != 0) {
		Log::error("Failed to set TCP_KEEPCNT to %i on %i: %s",
			   numProbes, fd, strerror(errno));
		return;
	}

	// The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes.
	if (setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &idleTime, sizeof(idleTime)) != 0) {
		Log::error("Failed to set TCP_KEEPIDLE to %i on %i: %s",
			   idleTime, fd, strerror(errno));
		return;
	}

	// The time (in seconds) between individual keepalive probes.
	if (setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) != 0) {
		Log::error("Failed to set TCP_KEEPINTVL to %i on %i: %s",
			   interval, fd, strerror(errno));
		return;
	}
}

uint64_t ntohll(uint64_t n) {
	#if __BYTE_ORDER == __BIG_ENDIAN
	return n;
	#else
	return (((uint64_t)ntohl(n)) << 32) + ntohl(n >> 32);
	#endif
}

uint64_t ntoh(uint64_t n)
{
	return ntohll(n);
}

uint32_t ntoh(uint32_t n)
{
	return ntohl(n);
}

uint16_t ntoh(uint16_t n)
{
	return ntohs(n);
}

uint8_t ntoh(uint8_t n)
{
	return n;
}

int16_t ntoh(int16_t n)
{
	return ntohs(n);
}

int32_t ntoh(int32_t n)
{
	return ntohl(n);
}

uint64_t htonll(uint64_t n) {
	#if __BYTE_ORDER == __BIG_ENDIAN
	return n;
	#else
	return (((uint64_t)htonl(n)) << 32) + htonl(n >> 32);
	#endif
}

uint64_t hton(uint64_t n)
{
	return htonll(n);
}

uint32_t hton(uint32_t n)
{
	return htonl(n);
}

uint16_t hton(uint16_t n)
{
	return htons(n);
}

uint8_t hton(uint8_t n)
{
	return n;
}

int16_t hton(int16_t n)
{
	return htons(n);
}

int32_t hton(int32_t n)
{
	return htonl(n);
}

template<class T>
bool recv(int fd, T& value)
{
	T netvalue;
	if ( recv( fd, (char*)&netvalue, sizeof(T), MSG_WAITALL) <= 0 )
	{
		return false;
	}
	value = ntoh(netvalue);
	return true;
}

template<class T>
bool send(int fd, T value)
{
	T netvalue = hton(value);
	return ( ::send(fd, (char*)&netvalue, sizeof(T), 0) == sizeof(T) );
}

template bool recv<uint8_t>(int fd, uint8_t& value);
template bool recv<uint16_t>(int fd, uint16_t& value);
template bool recv<uint32_t>(int fd, uint32_t& value);
template bool recv<uint64_t>(int fd, uint64_t& value);
template bool send<uint8_t>(int fd, uint8_t value);
template bool send<uint16_t>(int fd, uint16_t value);
template bool send<uint32_t>(int fd, uint32_t value);
template bool send<uint64_t>(int fd, uint64_t value);

template<class T> uint8_t* readvalue(T& value,uint8_t* buffer, uint32_t& buflen)
{
	if (buflen < sizeof(value)) __THROW__ ("Cannot read value from buffer - buffer too short!");
	value = ntoh(*((T*)buffer));
	buflen = buflen - sizeof(value);
	return buffer+sizeof(value);
}

template uint8_t* readvalue<uint8_t>(uint8_t& value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* readvalue<uint16_t>(uint16_t& value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* readvalue<uint32_t>(uint32_t& value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* readvalue<uint64_t>(uint64_t& value,uint8_t* buffer, uint32_t& buflen);

template<class T> uint8_t* writevalue(T value,uint8_t* buffer, uint32_t& buflen)
{
	if (buflen < sizeof(value)) __THROW__ ("Cannot write value to buffer - buffer too short!");
	*((T*)buffer) = hton(value);
	buflen = buflen - sizeof(value);
	return buffer+sizeof(value);
}

template uint8_t* writevalue<uint8_t>(uint8_t value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* writevalue<uint16_t>(uint16_t value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* writevalue<uint32_t>(uint32_t value,uint8_t* buffer, uint32_t& buflen);
template uint8_t* writevalue<uint64_t>(uint64_t value,uint8_t* buffer, uint32_t& buflen);

}}
