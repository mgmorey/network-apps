#ifndef NETWORK_STRINGRESULT_H
#define NETWORK_STRINGRESULT_H

#include "network/result.h"         // Result

#include <variant>      // std::variant

namespace Network
{
    using StringResult = std::variant<std::string, Result>;
}

#endif
