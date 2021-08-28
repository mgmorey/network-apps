#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-address.h"    // Address
#include "network-fd.h"         // sock_fd_type
#include "network-nullable.h"   // Nullable
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

namespace Network
{
    class Host
    {
    public:
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& ai);
        Host& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Host& t_addrinfo) const;
        bool operator>(const Host& t_addrinfo) const;
        bool operator==(const Host& t_addrinfo) const;
        operator Address() const;
        Nullable canonical_name() const;

    private:
        Address m_address;
        Nullable m_cname;
    };
}

#endif
