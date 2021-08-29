#include "network-socket.h"     // FdPair, Hostname, Result, Socket,
                                // SocketpairResult, SocketResult,
                                // sock_fd_type
#include "network-addrinfo.h"   // operator<<()
#include "network-family.h"     // Family
#include "network-format.h"     // Format
#include "network-host.h"       // Host
#include "network-protocol.h"   // Protocol
#include "network-socktype.h"   // SockType

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket(), socketpair()
#endif

#include <cassert>      // assert()
#include <cerrno>       // errno
#include <cstdlib>      // free()
#include <cstring>      // std::memcpy(), strdup(), std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Socket::Socket(int t_family,
                        int t_socktype,
                        int t_protocol,
                        int t_flags) :
    addrinfo(defaults(t_family,
                      t_socktype,
                      t_protocol,
                      t_flags))
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
    ai_canonname = nullptr;

    if (t_socket.ai_canonname != nullptr) {
        ai_canonname = strdup(t_socket.ai_canonname);
    }

    free(ai_addr);
    ai_addr = nullptr;

    if (t_socket.ai_addr != nullptr) {
        ai_addr = static_cast<sockaddr*>
            (malloc(ai_addrlen));
        std::memcpy(ai_addr, t_socket.ai_addr, ai_addrlen);
    }

    ai_next = nullptr;
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
    return Hostname(ai_canonname == nullptr ? "" : ai_canonname);
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

    errno = 0;
    std::string error;
    const sock_fd_type sock_fd = ::socket(ai_family,
                                          ai_socktype,
                                          ai_protocol);

    if (sock_fd == sock_fd_null) {
        std::ostringstream oss;
        oss << "socket("
            << Format("domain")
            << Family(ai_family)
            << Format(m_delim, m_tabs[0], "type")
            << SockType(ai_socktype)
            << Format(m_delim, m_tabs[0], "protocol")
            << Protocol(ai_family, ai_protocol)
            << ") returned "
            << sock_fd
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    Result result(errno, error);
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return SocketResult(sock_fd, result);
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

    errno = 0;
    std::string error;
    int sock_fd[2] = {sock_fd_null, sock_fd_null};
    const int code = ::socketpair(ai_family,
                                  ai_socktype,
                                  ai_protocol,
                                  sock_fd);

    if (code == socket_error) {
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

    FdPair fd_pair(sock_fd[0], sock_fd[1]);
    Result result(errno, error);
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return SocketpairResult(fd_pair, result);
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
        nullptr,	// ai_canonname
        nullptr,	// ai_addr
        nullptr		// ai_next
    };
    assert(ai.ai_flags == t_flags);
    assert(ai.ai_family == t_family);
    assert(ai.ai_socktype == t_socktype);
    assert(ai.ai_protocol == t_protocol);
    assert(ai.ai_addrlen == 0);
    assert(ai.ai_canonname == nullptr);
    assert(ai.ai_addr == nullptr);
    assert(ai.ai_next == nullptr);
    return ai;
}

const std::string Network::Socket::m_delim(", ");
const int Network::Socket::m_tabs[1] = {0};
