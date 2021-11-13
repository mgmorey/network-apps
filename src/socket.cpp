#include "network/socket.h"     // Hints, Host, Socket, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

Network::Socket::Socket(int t_family,
                        int t_socktype,
                        int t_protocol,
                        int t_flags) :
    Hints(t_family, t_socktype, t_protocol, t_flags)
{
}

Network::Socket::Socket(const addrinfo& t_addrinfo) :
    Hints(t_addrinfo),
    Host(t_addrinfo)
{
}

Network::Socket& Network::Socket::operator=(const addrinfo& t_addrinfo)
{
    static_cast<Hints>(*this) = t_addrinfo;
    static_cast<Host>(*this) = t_addrinfo;
    return *this;
}

bool Network::Socket::operator<(const Socket& t_sock) const
{
    return (static_cast<Hints>(*this) < t_sock ||
            static_cast<Host>(*this) < t_sock);
}

bool Network::Socket::operator>(const Socket& t_sock) const
{
    return (static_cast<Hints>(*this) > t_sock ||
            static_cast<Host>(*this) > t_sock);
}

bool Network::Socket::operator==(const Socket& t_sock) const
{
    return (static_cast<Hints>(*this) == t_sock &&
            static_cast<Host>(*this) == t_sock);
}
