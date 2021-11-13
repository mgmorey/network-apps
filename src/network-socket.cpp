#include "network/socket.h"     // FdPair, Hints, Host, Result,
                                // Socket, SocketpairResult,
                                // SocketResult, operator<<()
#include "network/error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network/format.h"     // Format

#ifdef _WIN32
#include <winsock2.h>   // sockaddr, socket()
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#include <sys/socket.h> // sockaddr, socket(), socketpair()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

static const std::string delim {", "};
static const int tab {0};

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

extern Network::SocketResult Network::get_socket(const Socket& sock,
                                                 bool verbose)
{
    Result result;

    if (verbose) {
        std::cerr << "Calling socket("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ')'
                  << std::endl;
    }

    reset_last_error();
    const auto fd {::socket(sock.family(), sock.socktype(), sock.protocol())};

    if (fd == fd_null) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socket("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return Fd(fd);
}

#ifndef _WIN32

extern Network::SocketpairResult Network::get_socketpair(const Socket& sock,
                                                         bool verbose)
{
    Result result;
    fd_type fds[] {fd_null, fd_null};

    if (verbose) {
        std::cerr << "Calling socketpair("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();
    auto code {::socketpair(sock.family(), sock.socktype(), sock.protocol(), fds)};

    if (code == socket_error) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return FdPair(fds[0], fds[1]);
}

#endif
