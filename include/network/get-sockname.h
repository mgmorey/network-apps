#ifndef NETWORK_GET_SOCKNAME_H
#define NETWORK_GET_SOCKNAME_H

#include "network/get-name.h"       // Fd, SockAddrResult

namespace Network
{
    extern SockAddrResult get_sockname(Fd fd, bool verbose = false);
}

#endif
