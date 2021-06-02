#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-types.h"

#ifdef _WIN32
#include <ws2tcpip.h>		// struct addrinfo, socklen_t
#else
#include <netdb.h>		// struct addrinfo
#endif

#include <utility>		// std::pair

typedef std::pair<int, Hostname> ConnectResult;

extern void close_socket(int fd);
extern ConnectResult connect_socket(const Hostname& host,
                                    const Service& service,
                                    const struct addrinfo &hints);

#endif
