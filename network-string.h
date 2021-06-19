#ifndef NETWORK_STRING_H
#define NETWORK_STRING_H

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, struct sockaddr, socklen_t
#else
#include <netdb.h>      // struct addrinfo, struct sockaddr
#include <sys/socket.h> // socklen_t
#endif

#include <cstddef>      // std::size_t
#include <string>       // std::string

extern std::string to_hex(const struct sockaddr* data, socklen_t size);
extern std::string to_hex(const char* data, std::size_t size);
extern std::string to_hex(const std::string& str);

#endif
