#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-types.h"

#ifdef _WIN32
#include <ws2tcpip.h>		// struct addrinfo
#else
#include <netdb.h>		// struct addrinfo
#endif

#include <list>			// std::list

typedef std::list<class Address> Addresses;

extern Addresses get_addresses(const Hostname& node,
                               const struct addrinfo* hints = NULL);
extern Addresses get_addresses(const Hostname& node,
                               const struct addrinfo& hints);

#endif
