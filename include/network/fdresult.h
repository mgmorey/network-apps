#ifndef NETWORK_FDRESULT_H
#define NETWORK_FDRESULT_H

#include "network/fd.h"             // Fd
#include "network/result.h"         // Result

#include <variant>      // std::variant

namespace Network
{
    using FdResult = std::variant<Fd, Result>;
}

#endif
