#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-types.h"

#ifdef _WIN32
#include <winsock2.h>		// AF_UNSPEC
#include <ws2tcpip.h>		// struct addrinfo, socklen_t
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// AF_UNSPEC, socklen_t
#endif

#include <list>			// std::list
#include <string>		// std::string
#include <utility>		// std::pair

typedef std::pair<int, Hostname> ConnectResult;

extern void close_socket(int fd);
extern ConnectResult connect_socket(const std::string& host,
                                    const std::string& service,
                                    const struct addrinfo &hints);

#endif
