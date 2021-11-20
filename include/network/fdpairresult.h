#ifndef NETWORK_FDPAIRRESULT_H
#define NETWORK_FDPAIRRESULT_H

#include "network/fdpair.h"         // FdPair
#include "network/result.h"         // Result

#include <variant>      // std::variant

namespace Network
{
    using FdPairResult = std::variant<FdPair, Result>;
}

#endif
