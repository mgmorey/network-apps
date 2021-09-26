#include "network-socket.h"     // Address, Hostname, Result, Socket,
                                // SocketFdPair, SocketpairResult,
                                // SocketResult
#include "network-addrinfo.h"   // operator<<()
#include "network-family.h"     // Family
#include "network-format.h"     // Format
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
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

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

bool Network::Socket::operator<(const Socket& t_socket) const
{
    return (static_cast<Hints>(*this) < t_socket ||
            static_cast<Host>(*this) < t_socket);
}

bool Network::Socket::operator>(const Socket& t_socket) const
{
    return (static_cast<Hints>(*this) > t_socket ||
            static_cast<Host>(*this) > t_socket);
}

bool Network::Socket::operator==(const Socket& t_socket) const
{
    return (static_cast<Hints>(*this) == t_socket &&
            static_cast<Host>(*this) == t_socket);
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
    const auto sock_fd {::socket(static_cast<int>(m_family),
                                 static_cast<int>(m_socktype),
                                 static_cast<int>(m_protocol))};

    if (sock_fd == sock_fd_null) {
        std::ostringstream oss;
        oss << "socket("
            << Format("domain")
            << m_family
            << Format(m_delim, m_tab, "type")
            << m_socktype
            << Format(m_delim, m_tab, "protocol")
            << m_protocol
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
    sock_fd_type fd[2] {sock_fd_null, sock_fd_null};
    const auto code {::socketpair(static_cast<int>(m_family),
                                  static_cast<int>(m_socktype),
                                  static_cast<int>(m_protocol),
                                  fd)};

    if (code == socket_error) {
        std::ostringstream oss;
        oss << "socketpair("
            << Format("domain")
            << m_family
            << Format(m_delim, m_tab, "type")
            << m_socktype
            << Format(m_delim, m_tab, "protocol")
            << m_protocol
            << ") returned "
            << code
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    SocketFdPair pair(fd[0], fd[1]);
    Result result(errno, error);
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return SocketpairResult(pair, result);
}

#endif

const std::string Network::Socket::m_delim {", "};
const int Network::Socket::m_tab {0};
