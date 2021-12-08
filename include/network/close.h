#ifndef NETWORK_CLOSE_H
#define NETWORK_CLOSE_H

#include "network/fd.h"             // Fd, fd_type

namespace Network
{
    extern fd_type close(fd_type fd);
    extern Fd close(Fd fd);
}

#endif
