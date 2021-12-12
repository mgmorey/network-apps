#ifndef NETWORK_CLOSE_H
#define NETWORK_CLOSE_H

#include "network/fd.h"             // Fd, fd_type

namespace Network
{
    extern auto close(fd_type fd) -> fd_type;
    extern auto close(Fd fd) -> Fd;
}

#endif
