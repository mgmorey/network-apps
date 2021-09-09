#ifndef NETWORK_HINTS_H
#define NETWORK_HINTS_H

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

namespace Network
{
    struct Hints
    {
        Hints(int t_family = 0,
              int t_socktype = 0,
              int t_protocol = 0,
              int t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Hints(const addrinfo& t_addrinfo);
        Hints& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Hints& t_hints) const;
        bool operator>(const Hints& t_hints) const;
        bool operator==(const Hints& t_hints) const;
        operator addrinfo() const;
        int flags() const;
        int family() const;
        int socktype() const;
        int protocol() const;

    protected:
        int m_flags;
        int m_family;
        int m_socktype;
        int m_protocol;
    };

}

#endif
