#ifndef REMOTE_PROTOCOLS_TCPUTIL_H
#define REMOTE_PROTOCOLS_TCPUTIL_H

#include "remote.h"

#include "protocols/types.h"
#include "protocols/MMSException.h"

namespace remote { namespace protocols {

using namespace std;
using namespace remote::util;

int openServerSocket(struct sockaddr_in& server, unsigned int port, int max_pending, int retryInterval);
int openClientSocket(std::string address, unsigned int port);
int nextClient( int serversock, sockaddr_in& client );

void setSendTimeout(int fd, long seconds, long microseconds );
void setKeepAlive( int fd, int numProbes, int idleTime, int interval);
void setSendBuffer( int fd, int byteSize);

in_addr_t resolve(const char *ip_addr);
const char *getHostByIp(in_addr ip);

template<class T> bool recv(int fd, T& value);
template<class T> bool send(int fd, T value);

template<class T> uint8_t* readvalue(T& value,uint8_t* buffer, uint32_t& buflen);
template<class T> uint8_t* writevalue(T value,uint8_t* buffer, uint32_t& buflen);

}}

#endif
