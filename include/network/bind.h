#ifndef NETWORK_BIND_H
#define NETWORK_BIND_H

#include "network/open.h"           // Endpoint, Fd, FdResult,
                                    // FdResultVector, Hints, Result,
                                    // SockAddr

namespace Network
{
    extern Result bind(Fd fd,
                       const SockAddr& addr,
                       bool verbose = false);
    extern FdResultVector bind(const Endpoint& endpoint,
                               const Hints* hints = nullptr,
                               bool verbose = false);
}

#endif
