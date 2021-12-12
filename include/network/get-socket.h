#ifndef NETWORK_GET_SOCKET_H
#define NETWORK_GET_SOCKET_H

#include "network/fdresult.h"       // FdResult
#include "network/socket.h"         // Socket

namespace Network
{
    extern auto get_socket(const Socket& sock,
                           bool verbose = false) -> FdResult;
}

#endif
