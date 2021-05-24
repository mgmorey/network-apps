#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>	// struct addrinfo

#include <string>	// std::string
#include <vector>	// std::vector

typedef std::string Value;
typedef std::vector<Value> Values;

Values getAddresses(const Value& host);
Value getHostname();
Value getNameInfo(const struct addrinfo& ai, int flags = 0);

#endif
