#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-socket.h"

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

typedef std::list<class Socket> Sockets;

extern Sockets get_sockets(const std::string& node,
                           const std::string& service = "",
                           const struct addrinfo* hints = NULL);
extern Sockets get_sockets(const std::string& node,
                           const std::string& service,
                           const struct addrinfo& hints);

#endif
