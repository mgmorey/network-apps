#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>		// PF_UNSPEC, struct addrinfo

#include <string>		// std::string
#include <unordered_set>	// std::unordered_set

typedef std::string Address;
typedef std::unordered_set<Address> Addresses;

Addresses getAddresses(int family = PF_UNSPEC,
                       int flags = 0);
Addresses getAddresses(const Address& host,
                       int family = PF_UNSPEC,
                       int flags = 0);
Address getHostname();
Address getNameInfo(const struct addrinfo& ai, int flags = 0);

#endif
