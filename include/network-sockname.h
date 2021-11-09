#ifndef NETWORK_SOCKNAME_H
#define NETWORK_SOCKNAME_H

#include "network-name.h"       // Fd, SockAddrResult

namespace Network
{
    extern SockAddrResult get_sockname(Fd fd, bool verbose = false);
}

#endif
