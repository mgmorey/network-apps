#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network/open.h"           // Endpoint, Fd, FdResultVector,
                                    // Hints, Result, SockAddr

namespace Network
{
    extern auto connect(Fd fd,
                        const SockAddr& addr,
                        bool verbose = false) -> Result;
    extern auto connect(const Endpoint& endpoint,
                        const Hints* hints = nullptr,
                        bool verbose = false) -> FdResultVector;
}

#endif
