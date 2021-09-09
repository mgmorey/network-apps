#ifndef NETWORK_HOSTS_H
#define NETWORK_HOSTS_H

#include "network-hints.h"      // Hints
#include "network-host.h"       // Host
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    typedef std::vector<Host> Hosts;
    typedef std::pair<Hosts, Result> HostsResult;
    extern HostsResult get_hosts(const Hostname& host,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
