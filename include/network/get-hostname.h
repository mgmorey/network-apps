#ifndef NETWORK_GET_HOSTNAME_H
#define NETWORK_GET_HOSTNAME_H

#include "network/stringresult.h"   // StringResult
#include "network/types.h"          // Hostname

#include <variant>      // std::variant

namespace Network
{
    using HostnameResult = StringResult;

    extern HostnameResult get_hostname(const Hostname& host = nullptr);
}

#endif
