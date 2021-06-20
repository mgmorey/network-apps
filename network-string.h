#ifndef NETWORK_STRING_H
#define NETWORK_STRING_H

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr
#include <ws2tcpip.h>   // struct addrinfo, socklen_t
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <cstddef>      // std::size_t
#include <string>       // std::string

extern std::string to_string(const struct sockaddr* sa, socklen_t len);
extern std::string to_string(const struct addrinfo& ai);
extern std::string to_string(const std::string& addr);

#endif
