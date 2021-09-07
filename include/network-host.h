#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-address.h"    // Address
#include "network-nullable.h"   // Nullable

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

namespace Network
{
    struct Host
    {
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo);
        bool operator<(const Host& t_host) const;
        bool operator>(const Host& t_host) const;
        bool operator==(const Host& t_host) const;
        Address address() const;
        Nullable canonical_name() const;

    private:
        Address m_address;
        Nullable m_canonname;
    };
}

#endif
