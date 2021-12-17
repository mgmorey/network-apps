#ifndef NETWORK_GET_HOSTNAME_H
#define NETWORK_GET_HOSTNAME_H

#include "network/hostname.h"           // Hostname
#include "network/hostnameresult.h"     // HostnameResult

#include <optional>     // std::nullopt

namespace Network
{
    extern auto get_hostname(const Hostname& host = std::nullopt) ->
        HostnameResult;
}

#endif
