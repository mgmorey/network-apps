#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-types.h"

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <list>         // std::list

namespace Network
{
    typedef std::list<class Socket> Sockets;

    extern Sockets get_sockets(const Hostname& node,
                               const Service& service = "",
                               const struct addrinfo* hints = NULL);
    extern Sockets get_sockets(const Hostname& node,
                               const Service& service,
                               const struct addrinfo& hints);
}

#endif
