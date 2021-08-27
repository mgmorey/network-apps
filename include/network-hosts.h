#ifndef NETWORK_HOSTS_H
#define NETWORK_HOSTS_H

#include "network-host.h"       // Host
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <list>         // std::list
#include <utility>      // std::pair

namespace Network
{
    typedef std::list<Host> Hosts;
    typedef std::pair<Hosts, Result> HostsResult;
    extern HostsResult get_hosts(const Hostname& host,
                                 const addrinfo* hints = NULL,
                                 bool verbose = false);
    extern HostsResult get_hosts(const Hostname& host,
                                 const addrinfo& hints,
                                 bool verbose = false);
}

#endif
