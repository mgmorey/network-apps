#include "network-socket.h"     // FdPair, Hints, Host, Result,
                                // Socket, SocketpairResult,
                                // SocketResult, operator<<()
#include "network-error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network-format.h"     // Format

#ifdef _WIN32
#include <winsock2.h>   // sockaddr, socket()
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#include <sys/socket.h> // sockaddr, socket(), socketpair()
#endif

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
    Result result;

    if (t_verbose) {
        std::cerr << "Calling socket("
                  << Format("domain")
                  << m_family
                  << Format(m_delim, m_tab, "type")
                  << m_socktype
                  << Format(m_delim, m_tab, "protocol")
                  << m_protocol
                  << ')'
                  << std::endl;
    }

    reset_last_error();
    const auto fd {::socket(m_family, m_socktype, m_protocol)};

    if (fd == fd_null) {
        auto error = get_last_error();
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
        return Result(error, oss.str());
    }

    return Fd(fd);
}

#ifndef _WIN32

Network::SocketpairResult Network::Socket::socketpair(bool t_verbose) const
{
    Result result;
    fd_type fds[] {fd_null, fd_null};

    if (t_verbose) {
        std::cerr << "Calling socketpair("
                  << Format("domain")
                  << m_family
                  << Format(m_delim, m_tab, "type")
                  << m_socktype
                  << Format(m_delim, m_tab, "protocol")
                  << m_protocol
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();
    auto code {::socketpair(m_family, m_socktype, m_protocol, fds)};

    if (code == socket_error) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << m_family
            << Format(m_delim, m_tab, "type")
            << m_socktype
            << Format(m_delim, m_tab, "protocol")
            << m_protocol
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return FdPair(fds[0], fds[1]);
}

#endif

const std::string Network::Socket::m_delim {", "};
const int Network::Socket::m_tab {0};
