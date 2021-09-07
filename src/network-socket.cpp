#include "network-socket.h"     // Address, FdPair, Hostname, Result,
                                // Socket, SocketpairResult,
                                // SocketResult, sock_fd_type
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
    m_flags(t_flags),
    m_family(t_family),
    m_socktype(t_socktype),
    m_protocol(t_protocol)
{
}

Network::Socket::Socket(const addrinfo& t_addrinfo) :
    m_flags(t_addrinfo.ai_flags),
    m_family(t_addrinfo.ai_family),
    m_socktype(t_addrinfo.ai_socktype),
    m_protocol(t_addrinfo.ai_protocol),
    m_address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen),
    m_canonname(t_addrinfo.ai_canonname)
{
}

Network::Socket& Network::Socket::operator=(const addrinfo& t_addrinfo)
{
    m_flags = t_addrinfo.ai_flags;
    m_family = t_addrinfo.ai_family;
    m_socktype = t_addrinfo.ai_socktype;
    m_protocol = t_addrinfo.ai_protocol;
    m_address = Address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen);
    m_canonname = t_addrinfo.ai_canonname;
    return *this;
}

bool Network::Socket::operator<(const Socket& t_socket) const
{
    return (m_protocol < t_socket.m_protocol ||
            m_socktype < t_socket.m_socktype ||
            m_family < t_socket.m_family ||
            m_address < t_socket.m_address);
}

bool Network::Socket::operator>(const Socket& t_socket) const
{
    return (m_protocol > t_socket.m_protocol ||
            m_socktype > t_socket.m_socktype ||
            m_family > t_socket.m_family ||
            m_address > t_socket.m_address);
}

bool Network::Socket::operator==(const Socket& t_socket) const
{
    return (m_protocol == t_socket.m_protocol &&
            m_socktype == t_socket.m_socktype &&
            m_family == t_socket.m_family &&
            m_address == t_socket.m_address);
}

Network::Socket::operator addrinfo() const
{
    addrinfo ai = {
        m_flags,
        m_family,
        m_socktype,
        m_protocol,
        0,
        nullptr,
        nullptr,
        nullptr
    };
    return ai;
}

Network::Address Network::Socket::address() const
{
    return m_address;
}

Network::Nullable Network::Socket::canonical_name() const
{
    return m_canonname;
}

int Network::Socket::family() const
{
    return m_family;
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
    const sock_fd_type sock_fd = ::socket(m_family,
                                          m_socktype,
                                          m_protocol);

    if (sock_fd == sock_fd_null) {
        std::ostringstream oss;
        oss << "socket("
            << Format("domain")
            << Family(m_family)
            << Format(m_delim, m_tabs[0], "type")
            << SockType(m_socktype)
            << Format(m_delim, m_tabs[0], "protocol")
            << Protocol(m_family, m_protocol)
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
    const int code = ::socketpair(m_family,
                                  m_socktype,
                                  m_protocol,
                                  sock_fd);

    if (code == socket_error) {
        std::ostringstream oss;
        oss << "socketpair("
            << Format("domain")
            << Family(m_family)
            << Format(m_delim, m_tabs[0], "type")
            << SockType(m_socktype)
            << Format(m_delim, m_tabs[0], "protocol")
            << Protocol(m_family, m_protocol)
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

const std::string Network::Socket::m_delim(", ");
const int Network::Socket::m_tabs[1] = {0};
