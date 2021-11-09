#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-open.h"       // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResult,
                                // SocketResults, vector

namespace Network
{
    extern Result connect(Fd fd,
                          const SockAddr& sock_addr,
                          bool verbose = false);
    extern SocketResults connect(const Endpoint& endpoint,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
