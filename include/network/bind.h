#ifndef NETWORK_BIND_H
#define NETWORK_BIND_H

#include "network/open.h"           // Bytes, Endpoint, Fd,
                                    // FdResultVector, Hints, Result

namespace Network
{
    extern auto bind(Fd fd,
                     const Bytes& addr,
                     bool verbose = false) -> Result;
    extern auto bind(const Endpoint& endpoint,
                     const Hints* hints = nullptr,
                     bool verbose = false) -> FdResultVector ;
}

#endif
