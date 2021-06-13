#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<int, std::string> ConnectResult;

    extern void close_socket(int fd);
    extern ConnectResult connect_socket(const Hostname& host,
                                        const Service& service,
                                        const struct addrinfo &hints);
}

#endif
