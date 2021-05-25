#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// PF_UNSPEC

#include <set>			// std::set
#include <string>		// std::string

typedef std::string Address;
typedef std::set<Address> Addresses;
typedef std::string HostName;
typedef std::string NameInfo;

Addresses getAddresses(int family = PF_UNSPEC,
                       int flags = 0);
Addresses getAddresses(const Address& host,
                       int family = PF_UNSPEC,
                       int flags = 0);
HostName getHostname();
NameInfo getNameInfo(const struct addrinfo& ai, int flags = 0);

#endif
