#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-address.h"    // Address
#include "network-nullable.h"   // Nullable
#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname

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
        operator Address() const;
        Nullable canonical_name() const;

    private:
        Nullable m_canonname;
        Address m_address;
    };
}

#endif
