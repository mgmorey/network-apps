#ifndef NETWORK_CLOSE_H
#define NETWORK_CLOSE_H

#include "network-fd.h"         // sock_fd_type

namespace Network
{
    extern sock_fd_type close(sock_fd_type sock_fd);
}

#endif
