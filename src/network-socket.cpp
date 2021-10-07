#include "network-socket.h"     // Address, Hostname, Result, Socket,
                                // FdPair, SocketpairResult,
                                // SocketResult
#include "network-addrinfo.h"   // operator<<()
#include "network-error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network-family.h"     // Family
#include "network-format.h"     // Format
#include "network-protocol.h"   // Protocol
#include "network-socktype.h"   // SockType

#ifdef _WIN32
#include <winsock2.h>   // sockaddr, socket()
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#include <sys/socket.h> // sockaddr, socket(), socketpair()
#endif

#include <cassert>      // assert()
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

    std::string message;
    auto error {reset_last_error()};
    auto fd {::socket(static_cast<int>(m_family),
                      static_cast<int>(m_socktype),
                      static_cast<int>(m_protocol))};

    if (fd == fd_null) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socket("
            << Format("domain")
            << m_family
            << Format(m_delim, m_tab, "type")
            << m_socktype
            << Format(m_delim, m_tab, "protocol")
            << m_protocol
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    const Result result(error, message);
    return SocketResult(fd, result);
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

    std::string message;
    fd_type fds[] {fd_null, fd_null};
    auto error {reset_last_error()};
    auto code {::socketpair(static_cast<int>(m_family),
                             static_cast<int>(m_socktype),
                             static_cast<int>(m_protocol),
                             fds)};

    if (code == socket_error) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << m_family
            << Format(m_delim, m_tab, "type")
            << m_socktype
            << Format(m_delim, m_tab, "protocol")
            << m_protocol
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    return SocketpairResult(FdPair(fds[0], fds[1]), {error, message});
}

#endif

const std::string Network::Socket::m_delim {", "};
const int Network::Socket::m_tab {0};
