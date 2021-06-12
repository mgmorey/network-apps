#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-address.h"    // Address
#include "network-hostname.h"   // get_hostname()
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

    extern Addresses get_addresses(const Network::Hostname& node =
                                   Network::get_hostname(),
                                   const Network::Service& service = "",
                                   const struct addrinfo& hints =
                                   Network::Socket(AF_UNSPEC));
}

#endif
