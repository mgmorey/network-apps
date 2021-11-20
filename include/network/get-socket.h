#ifndef NETWORK_GET_SOCKET_H
#define NETWORK_GET_SOCKET_H

#include "network/fdresult.h"       // Fd, FdResult, Result
#include "network/socket.h"         // Socket

namespace Network
{
    extern FdResult get_socket(const Socket& sock,
                               bool verbose = false);
}

#endif
