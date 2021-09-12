#ifndef NETWORK_CLOSE_H
#define NETWORK_CLOSE_H

#include "network-fd.h"         // SocketFd

namespace Network
{
    extern SocketFd close(SocketFd socket_fd);
}

#endif
