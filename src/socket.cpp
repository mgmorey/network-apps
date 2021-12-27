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

auto Network::Socket::operator=(const addrinfo& t_addrinfo) -> Network::Socket&
{
    static_cast<Hints>(*this) = t_addrinfo;  // NOLINT
    static_cast<Host>(*this) = t_addrinfo;  // NOLINT
    return *this;
}

auto Network::Socket::operator<(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) < t_sock ||  // NOLINT
            static_cast<Host>(*this) < t_sock);  // NOLINT
}

auto Network::Socket::operator>(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) > t_sock ||  // NOLINT
            static_cast<Host>(*this) > t_sock);  // NOLINT
}

auto Network::Socket::operator==(const Socket& t_sock) const -> bool
{
    return (static_cast<Hints>(*this) == t_sock &&  // NOLINT
            static_cast<Host>(*this) == t_sock);  // NOLINT
}
