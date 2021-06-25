#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service
#include "network-sockets.h"    // Sockets, SocketsResult

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
    extern ConnectResult connect(const Endpoint& endpoint,
                                 const addrinfo &hints,
                                 bool verbose = false);
    extern ConnectResult connect(const Sockets& sockets,
                                 bool verbose = false);
    extern Result connect(int fd, const addrinfo& ai,
                          bool verbose = false);
}

#endif
