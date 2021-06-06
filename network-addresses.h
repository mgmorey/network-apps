#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-address.h"    // Address
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <list>         // std::list

namespace Network
{
    typedef std::list<Address> Addresses;

    extern Addresses get_addresses(const Hostname& node,
                                   const struct addrinfo* hints = NULL);
    extern Addresses get_addresses(const Hostname& node,
                                   const struct addrinfo& hints);
}

#endif
