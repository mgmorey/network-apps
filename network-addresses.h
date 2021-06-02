#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-address.h"

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

typedef std::list<class Address> Addresses;

extern Addresses get_addresses(const std::string& node,
                               const struct addrinfo* hints = NULL);
extern Addresses get_addresses(const std::string& node,
                               const struct addrinfo& hints);

#endif
