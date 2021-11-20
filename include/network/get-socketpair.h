#ifndef NETWORK_GET_SOCKETPAIR_H
#define NETWORK_GET_SOCKETPAIR_H

#include "network/fdpair.h"         // FdPair
#include "network/socket.h"         // Socket
#include "network/result.h"         // Result

#include <variant>      // std::variant

namespace Network
{
    using FdPairResult = std::variant<FdPair, Result>;

#ifndef _WIN32
    extern FdPairResult get_socketpair(const Socket& sock,
                                       bool verbose = false);
#endif
}

#endif
