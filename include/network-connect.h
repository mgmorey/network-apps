#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // type_fd
#include "network-result.h"     // Result
#include "network-sockets.h"    // Sockets

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <vector>       // std::vector
#include <utility>      // std::pair

namespace Network
{
    typedef std::vector<Result> ConnectDetails;
    typedef std::pair<fd_type, ConnectDetails> ConnectResult;
    extern ConnectResult connect(const Endpoint& endpoint,
                                 const addrinfo &hints,
                                 bool verbose = false);
    extern ConnectResult connect(const Sockets& sockets,
                                 bool verbose = false);
}

#endif
