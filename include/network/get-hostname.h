#ifndef NETWORK_GET_HOSTNAME_H
#define NETWORK_GET_HOSTNAME_H

#include "network/hostnameresult.h"     // HostnameResult
#include "network/string-types.h"       // Hostname

#include <optional>     // std::nullopt

namespace Network
{
    extern auto get_hostname(const Hostname& host = std::nullopt) ->
        HostnameResult;
}

#endif
