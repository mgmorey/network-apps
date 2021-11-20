#ifndef NETWORK_GET_SOCKET_H
#define NETWORK_GET_SOCKET_H

#include "network/fdresult.h"       // Fd, FdResult, Result
#include "network/socket.h"         // Socket

#include <variant>      // std::variant

namespace Network
{
    using FdResult = std::variant<Fd, Result>;

    extern FdResult get_socket(const Socket& sock,
                               bool verbose = false);
}

#endif
