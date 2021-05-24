#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>	// PF_UNSPEC, struct addrinfo

#include <set>		// std::set
#include <string>	// std::string

typedef std::string Address;
typedef std::set<Address> Addresses;

Addresses getAddresses(int family = PF_UNSPEC,
                       int flags = 0);
Addresses getAddresses(const Address& host,
                       int family = PF_UNSPEC,
                       int flags = 0);
Address getHostname();
Address getNameInfo(const struct addrinfo& ai, int flags = 0);

#endif
