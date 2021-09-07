#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network-address.h"    // Address
#include "network-fd.h"         // sock_fd_type
#include "network-nullable.h"   // Nullable
#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // SOCKET_ERROR
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string
#include <utility>      // std::pair

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	(-1)
#endif

namespace Network
{
    typedef std::pair<sock_fd_type, sock_fd_type> FdPair;
    typedef std::pair<sock_fd_type, Result> SocketResult;
    typedef std::pair<FdPair, Result> SocketpairResult;

    enum { socket_error = SOCKET_ERROR };

    struct Socket
    {
        Socket(int t_family = 0,
               int t_socktype = 0,
               int t_protocol = 0,
               int t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Socket(const addrinfo& t_addrinfo);
        Socket& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Socket& t_socket) const;
        bool operator>(const Socket& t_socket) const;
        bool operator==(const Socket& t_socket) const;
        operator addrinfo() const;
        Address address() const;
        Nullable canonical_name() const;
        int family() const;
        SocketResult socket(bool t_verbose = false) const;
#ifndef _WIN32
        SocketpairResult socketpair(bool t_verbose = false) const;
#endif

    private:
        static const std::string m_delim;
        static const int m_tabs[1];

        int m_flags;
        int m_family;
        int m_socktype;
        int m_protocol;
        Address m_address;
        Nullable m_canonname;
    };

}

#endif
