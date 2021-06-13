#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-address.h"    // Address
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
    typedef std::list<Address> Addresses;

    extern Addresses get_addresses(std::string& error,
                                   const Hostname& host = "",
                                   const Service& service = "",
                                   const struct addrinfo& hints =
                                   Socket(AF_UNSPEC));
}

#endif
