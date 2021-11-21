#ifndef NETWORK_GET_PEERNAME_H
#define NETWORK_GET_PEERNAME_H

#include "network/get-name.h"       // Fd, SockAddrResult

namespace Network
{
    extern SockAddrResult get_peername(Fd fd, bool verbose = false);
}

#endif
