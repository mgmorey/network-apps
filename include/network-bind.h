#ifndef NETWORK_BIND_H
#define NETWORK_BIND_H

#include "network-open.h"       // Endpoint, Fd, Hints, Result,
                                // SockAddr, SocketResult,
                                // SocketResults, vector

namespace Network
{
    extern Result bind(const Fd& fd,
                       const SockAddr& sock_addr,
                       bool verbose = false);
    extern SocketResults bind(const Endpoint& endpoint,
                              const Hints* hints = nullptr,
                              bool verbose = false);
}

#endif
