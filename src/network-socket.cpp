#include "network-socket.h"     // Hostname, Result, Socket
#include "network-addrinfo.h"   // operator<<()
#include "network-family.h"     // Family
#include "network-format.h"     // Format
#include "network-protocol.h"   // Protocol
#include "network-socktype.h"   // SockType
#include "network-host.h"       // Host

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket()
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstdlib>      // free()
#include <cstring>      // std::memcpy(), strdup(), std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Socket::Socket(int t_family, int t_socktype, int t_protocol, int t_flags) :
    addrinfo(defaults(t_family, t_socktype, t_protocol, t_flags))
{
}

Network::Socket::Socket(const Socket& t_socket) :
    addrinfo(defaults())
{
    *this = t_socket;
}

Network::Socket::Socket(const addrinfo& t_socket) :
    addrinfo(defaults())
{
    *this = t_socket;
}

Network::Socket::~Socket()
{
    free(ai_addr);
    free(ai_canonname);
}

Network::Socket& Network::Socket::operator=(const Socket& t_socket)
{
    return *this = static_cast<const addrinfo&>(t_socket);
}

Network::Socket& Network::Socket::operator=(const addrinfo& t_socket)
{
    ai_flags = t_socket.ai_flags;
    ai_family = t_socket.ai_family;
    ai_socktype = t_socket.ai_socktype;
    ai_protocol = t_socket.ai_protocol;
    ai_addrlen = t_socket.ai_addrlen;
    free(ai_canonname);
    ai_canonname = NULL;

    if (t_socket.ai_canonname != NULL) {
        ai_canonname = strdup(t_socket.ai_canonname);
    }

    free(ai_addr);
    ai_addr = NULL;

    if (t_socket.ai_addr != NULL) {
        ai_addr = static_cast<sockaddr*>
            (malloc(ai_addrlen));
        std::memcpy(ai_addr, t_socket.ai_addr, ai_addrlen);
    }

    ai_next = NULL;
    return *this;
}

bool Network::Socket::operator<(const Socket& t_socket) const
{
    return (ai_protocol < t_socket.ai_protocol ||
            ai_socktype < t_socket.ai_socktype ||
            ai_family < t_socket.ai_family ||
            Host(*this) < Host(t_socket));
}

bool Network::Socket::operator>(const Socket& t_socket) const
{
    return (ai_protocol > t_socket.ai_protocol ||
            ai_socktype > t_socket.ai_socktype ||
            ai_family > t_socket.ai_family ||
            Host(*this) > Host(t_socket));
}

bool Network::Socket::operator==(const Socket& t_socket) const
{
    return (ai_protocol == t_socket.ai_protocol &&
            ai_socktype == t_socket.ai_socktype &&
            ai_family == t_socket.ai_family &&
            Host(*this) == Host(t_socket));
}

Network::Hostname Network::Socket::cname() const
{
    return Hostname(ai_canonname == NULL ? "" : ai_canonname);
}

int Network::Socket::family() const
{
    return ai_family;
}

Network::SocketResult Network::Socket::socket(bool t_verbose) const
{
    if (t_verbose) {
        std::cerr << "Trying socket:"
                  << std::endl
                  << *this
                  << std::endl;
    }

    std::string error;
    const sock_fd_type fd = ::socket(ai_family, ai_socktype, ai_protocol);

    if (fd == sock_fd_null) {
        std::ostringstream oss;
        oss << "socket("
            << Format("domain")
            << Family(ai_family)
            << Format(m_delim, m_tabs[0], "type")
            << SockType(ai_socktype)
            << Format(m_delim, m_tabs[0], "protocol")
            << Protocol(ai_family, ai_protocol)
            << ") returned "
            << fd
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    return SocketResult(fd, Result(0, error));
}

#ifndef _WIN32
Network::SocketpairResult Network::Socket::socketpair(bool t_verbose) const
{
    if (t_verbose) {
        std::cerr << "Trying socket:"
                  << std::endl
                  << *this
                  << std::endl;
    }

    std::string error;
    int fds[2] = {sock_fd_null, sock_fd_null};
    const int code = ::socketpair(ai_family, ai_socktype, ai_protocol, fds);

    if (code != 0) {
        std::ostringstream oss;
        oss << "socketpair("
            << Format("domain")
            << Family(ai_family)
            << Format(m_delim, m_tabs[0], "type")
            << SockType(ai_socktype)
            << Format(m_delim, m_tabs[0], "protocol")
            << Protocol(ai_family, ai_protocol)
            << ") returned "
            << code
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    return SocketpairResult(FdPair(fds[0], fds[1]),
                            Result(code, error));
}
#endif

addrinfo Network::Socket::defaults(int t_family,
                                   int t_socktype,
                                   int t_protocol,
                                   int t_flags)
{
    const addrinfo ai = {
        t_flags,	// ai_flags
        t_family,	// ai_family
        t_socktype,	// ai_socktype
        t_protocol,	// ai_protocol
        0,		// ai_addrlen
        NULL,		// ai_canonname
        NULL,		// ai_addr
        NULL		// ai_next
    };
    assert(ai.ai_flags == t_flags);
    assert(ai.ai_family == t_family);
    assert(ai.ai_socktype == t_socktype);
    assert(ai.ai_protocol == t_protocol);
    assert(ai.ai_addrlen == 0);
    assert(ai.ai_canonname == NULL);
    assert(ai.ai_addr == NULL);
    assert(ai.ai_next == NULL);
    return ai;
}

const std::string Network::Socket::m_delim(", ");
const int Network::Socket::m_tabs[1] = {0};
