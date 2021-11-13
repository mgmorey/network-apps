#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network/name.h"       // Fd, SockAddrResult

namespace Network
{
    extern SockAddrResult get_peername(Fd fd, bool verbose = false);
}

#endif
