#ifndef NETWORK_CLOSE_H
#define NETWORK_CLOSE_H

#include "network-fd.h"         // Fd

namespace Network
{
    extern Fd close(Fd socket_fd);
}

#endif
