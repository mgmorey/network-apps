#ifndef NETWORK_HOSTS_H
#define NETWORK_HOSTS_H

#include "network/hints.h"          // Hints
#include "network/host.h"           // Host
#include "network/result.h"         // Result
#include "network/types.h"          // Hostname

#include <variant>      // std::variant
#include <vector>       // std::vector

namespace Network
{
    using Hosts = std::vector<Host>;
    using HostsResult = std::variant<Hosts, Result>;

    extern HostsResult get_hosts(const Hostname& host = nullptr,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
