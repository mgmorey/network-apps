#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network/open.h"           // Endpoint, Fd, FdResult,
                                    // FdResults, Hints, Result,
                                    // SockAddr, vector

namespace Network
{
    extern Result connect(Fd fd,
                          const SockAddr& addr,
                          bool verbose = false);
    extern FdResults connect(const Endpoint& endpoint,
                             const Hints* hints = nullptr,
                             bool verbose = false);
}

#endif
