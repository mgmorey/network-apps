#include "network-socket.h"     // Hostname, Result, Socket
#include "network-addrinfo.h"   // operator<<()
#include "network-host.h"       // Host

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket()
#endif

#include <cstdlib>      // free()
#include <cstring>      // std::memcpy(), strdup()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Socket::Socket(int family, int socktype, int protocol, int flags) :
    addrinfo(defaults(family, socktype, protocol, flags))
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
            Host(*this) < Host(other));
}

bool Network::Socket::operator>(const Socket& other) const
{
    return (ai_protocol > other.ai_protocol ||
            ai_socktype > other.ai_socktype ||
            ai_family > other.ai_family ||
            Host(*this) > Host(other));
}

bool Network::Socket::operator==(const Socket& other) const
{
    return (ai_protocol == other.ai_protocol &&
            ai_socktype == other.ai_socktype &&
            ai_family == other.ai_family &&
            Host(*this) == Host(other));
}

Network::Hostname Network::Socket::cname() const
{
    Hostname host(ai_canonname == NULL ? "" : ai_canonname);
    return host;
}

Network::SocketResult Network::Socket::socket(bool verbose) const
{
    if (verbose) {
        std::cerr << "Trying socket:"
                  << std::endl
                  << *this
                  << std::endl;
    }

    std::string error;
    fd_type fd = ::socket(ai_family, ai_socktype, ai_protocol);

    if (fd == fd_null) {
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

    return SocketResult(fd, Result(0, error));
}

#ifndef _WIN32
Network::SocketPairResult Network::Socket::socketpair(bool verbose) const
{
    if (verbose) {
        std::cerr << "Trying socket:"
                  << std::endl
                  << *this
                  << std::endl;
    }

    std::string error;
    int fds[2] = {fd_null, fd_null};
    int code = ::socketpair(ai_family, ai_socktype, ai_protocol, fds);

    if (code != 0) {
        std::ostringstream os;
        os << "socketpair("
           << ai_family
           << ", "
           << ai_socktype
           << ", "
           << ai_protocol
           << ") returned "
           << code;
        error = os.str();
    }

    return SocketPairResult(SocketPair(fds[0], fds[1]),
                            Result(code, error));
}
#endif

addrinfo Network::Socket::defaults(int family,
                                   int socktype,
                                   int protocol,
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
