#ifndef REMOTE_REMOTE_H
#define REMOTE_REMOTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>

#include "util/File.h"
#include "util/Log.h"
#include "util/Config.h"
#include "util/error.h"

using namespace remote::util;

namespace remote {
	typedef uint32_t dbkey_t;
	typedef uint8_t result_t;
	typedef uint8_t status_t;
}

#endif
