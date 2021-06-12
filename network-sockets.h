#ifndef NETWORK_SOCKETS_H
#define NETWORK_SOCKETS_H

#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <list>         // std::list

namespace Network
{
    typedef std::list<Socket> Sockets;

    extern Sockets get_sockets(const Hostname& hostname,
                               const Service& service,
                               const struct addrinfo& hints);
}

#endif
