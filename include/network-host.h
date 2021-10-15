#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-types.h"      // Hostname, SockAddr

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Host
    {
        Host();
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo);
        Host& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Host& t_host) const;
        bool operator>(const Host& t_host) const;
        bool operator==(const Host& t_host) const;
        SockAddr address() const;
        Hostname canonical_name() const;

    protected:
        SockAddr m_sock_addr;
        Hostname m_canonname;

    public:
        static SockAddr get_sockaddr(const addrinfo& ai);
    };
}

#endif
