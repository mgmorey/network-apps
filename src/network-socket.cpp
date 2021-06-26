#include "network-socket.h"     // Hostname, Result, Socket
#include "network-address.h"    // Address

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket()
#endif

#include <cstdlib>      // free()
#include <cstring>      // std::memcpy(), strdup()
#include <sstream>      // std::ostringstream

Network::Socket::Socket(int protocol, int socktype, int family, int flags) :
    addrinfo(defaults(protocol, socktype, family, flags))
{
}

Network::Socket::Socket(const Socket& other) :
    addrinfo(defaults())
{
    *this = other;
}

Network::Socket::Socket(const addrinfo& other) :
    addrinfo(defaults())
{
    *this = other;
}

Network::Socket::~Socket()
{
    free(ai_addr);
    free(ai_canonname);
}

Network::Socket& Network::Socket::operator=(const Socket& other)
{
    return *this = static_cast<const addrinfo&>(other);
}

Network::Socket& Network::Socket::operator=(const addrinfo& other)
{
    ai_flags = other.ai_flags;
    ai_family = other.ai_family;
    ai_socktype = other.ai_socktype;
    ai_protocol = other.ai_protocol;
    ai_addrlen = other.ai_addrlen;
    free(ai_canonname);
    ai_canonname = NULL;

    if (other.ai_canonname != NULL) {
        ai_canonname = strdup(other.ai_canonname);
    }

    free(ai_addr);
    ai_addr = NULL;

    if (other.ai_addr != NULL) {
        ai_addr = static_cast<sockaddr*>
            (malloc(ai_addrlen));
        std::memcpy(ai_addr, other.ai_addr, ai_addrlen);
    }

    ai_next = NULL;
    return *this;
}

bool Network::Socket::operator<(const Socket& other) const
{
    return (ai_protocol < other.ai_protocol ||
            ai_socktype < other.ai_socktype ||
            ai_family < other.ai_family ||
            Address(*this) < Address(other));
}

bool Network::Socket::operator>(const Socket& other) const
{
    return (ai_protocol > other.ai_protocol ||
            ai_socktype > other.ai_socktype ||
            ai_family > other.ai_family ||
            Address(*this) > Address(other));
}

bool Network::Socket::operator==(const Socket& other) const
{
    return (ai_protocol == other.ai_protocol &&
            ai_socktype == other.ai_socktype &&
            ai_family == other.ai_family &&
            Address(*this) == Address(other));
}

Network::Hostname Network::Socket::cname() const
{
    Hostname host(ai_canonname == NULL ? "" : ai_canonname);
    return host;
}

Network::Result Network::Socket::socket() const
{
    std::string error;
    int fd = ::socket(ai_family, ai_socktype, ai_protocol);

    if (fd == socket_bad) {
        std::ostringstream os;
        os << "socket("
           << ai_family
           << ", "
           << ai_socktype
           << ", "
           << ai_protocol
           << ") returned "
           << fd;
        error = os.str();
    }

    return Result(fd, error);
}

addrinfo Network::Socket::defaults(int protocol,
                                   int socktype,
                                   int family,
                                   int flags)
{
    addrinfo ai = {
        flags,		// ai_flags
        family,		// ai_family
        socktype,	// ai_socktype
        protocol,	// ai_protocol
        0,		// ai_addrlen
        NULL,		// ai_addr
        NULL,		// ai_canonname
        NULL		// ai_next
    };
    return ai;
}
