#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network-fd.h"         // fd_type
#include "network-result.h"     // Result
#include "network-sockaddr.h"   // SockAddr

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<SockAddr, Result> SockAddrResult;
    extern SockAddrResult get_peername(fd_type fd, bool verbose = false);
}

#endif
