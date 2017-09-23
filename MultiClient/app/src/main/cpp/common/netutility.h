#ifndef _net_utility_h_2016_12_30_lc_
#define _net_utility_h_2016_12_30_lc_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include <netinet/tcp.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

typedef int SOCKET;

typedef sockaddr_in SOCKADDR_IN;

#endif