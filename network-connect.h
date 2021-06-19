#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string
#include <vector>       // std::vector
#include <utility>      // std::pair

namespace Network
{
    typedef std::vector<std::string> ConnectDetails;
    typedef std::pair<int, ConnectDetails> ConnectResult;
    extern void close(int fd);
    extern ConnectResult connect(const Hostname& hostname,
                                 const Service& service,
                                 const struct addrinfo &hints,
                                 bool is_verbose = false);
}

#endif
