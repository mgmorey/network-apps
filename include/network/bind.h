#ifndef NETWORK_BIND_H
#define NETWORK_BIND_H

#include "network/open.h"           // Endpoint, Fd, FdResultVector,
                                    // Hints, Result, SockAddr

namespace Network
{
    extern auto bind(Fd fd,
                     const SockAddr& addr,
                     bool verbose = false) -> Result;
    extern auto bind(const Endpoint& endpoint,
                     const Hints* hints = nullptr,
                     bool verbose = false) -> FdResultVector ;
}

#endif
