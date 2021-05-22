#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>	// struct addrinfo

#include <string>	// std::string
#include <vector>	// std::vector

typedef std::string Value;
typedef std::vector<Value> Values;

Value getHostName(const struct addrinfo* ai = NULL, int flags = 0);
Values getIpAddress(const Value& host);

#endif
