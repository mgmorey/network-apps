#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>	// PF_UNSPEC, struct addrinfo

#include <string>	// std::string
#include <vector>	// std::vector

typedef std::string Value;
typedef std::vector<Value> Values;

Values getAddresses(int family = PF_UNSPEC,
                    int flags = 0);
Values getAddresses(const Value& host,
                    int family = PF_UNSPEC,
                    int flags = 0);
Value getHostname();
Value getNameInfo(const struct addrinfo& ai, int flags = 0);

#endif
