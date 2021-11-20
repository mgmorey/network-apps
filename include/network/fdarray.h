#ifndef NETWORK_FDARRAY_H
#define NETWORK_FDARRAY_H

#include "network/fd.h"             // Fd

#include <array>        // std::array

namespace Network
{
    using FdArray = std::array<Fd, 2>;
}

#endif
