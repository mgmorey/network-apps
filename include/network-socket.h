#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network-fd.h"         // sock_fd_type
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<sock_fd_type, sock_fd_type> FdPair;
    typedef std::pair<sock_fd_type, Result> SocketResult;
    typedef std::pair<FdPair, Result> SocketpairResult;

    class Socket :
        public addrinfo
    {
    public:
        Socket(int t_family = 0,
               int t_socktype = 0,
               int t_protocol = 0,
               int t_flags = 0);
        Socket(const Socket& t_socket);
        Socket(const addrinfo& t_socket);
        ~Socket();
        Socket& operator=(const Socket& t_socket);
        Socket& operator=(const addrinfo& t_socket);
        bool operator<(const Socket& t_socket) const;
        bool operator>(const Socket& t_socket) const;
        bool operator==(const Socket& t_socket) const;
        Hostname cname() const;
        int family() const;
        SocketResult socket(bool t_verbose = false) const;
#ifndef _WIN32
        SocketpairResult socketpair(bool t_verbose = false) const;
#endif

    private:
        static addrinfo defaults(int t_family = 0,
                                 int t_socktype = 0,
                                 int t_protocol = 0,
                                 int t_flags = 0);

        static const std::string m_delim;
        static const int m_tabs[1];
    };
}

#endif
