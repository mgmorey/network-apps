#ifndef NETWORK_FDPAIR_H
#define NETWORK_FDPAIR_H

#include "network/fd.h"             // Fd

#include <array>        // std::array

namespace Network
{
    using FdPair = std::array<Fd, 2>;
}

#endif
