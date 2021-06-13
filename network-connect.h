#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

namespace Network
{
    extern void close_socket(int fd);
    extern Result connect_socket(const Hostname& host,
                                 const Service& service,
                                 const struct addrinfo &hints);
}

#endif
