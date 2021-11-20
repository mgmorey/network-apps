#ifndef NETWORK_GET_SOCKETPAIR_H
#define NETWORK_GET_SOCKETPAIR_H

#include "network/fdpairresult.h"   // FdPairResult
#include "network/socket.h"         // Socket

namespace Network
{
#ifndef _WIN32
    extern FdPairResult get_socketpair(const Socket& sock,
                                       bool verbose = false);
#endif
}

#endif
