#ifndef NETWORK_ADDRESSES_H
#define NETWORK_ADDRESSES_H

#include "network-address.h"    // Address
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
    typedef std::list<Address> Addresses;
    typedef std::pair<Addresses, Result> AddressesResult;
    extern AddressesResult get_addresses(const Hostname& host = "",
                                         const Service& service = "",
                                         const struct addrinfo& hints =
                                         Socket(),
                                         bool verbose = false);
}

#endif
