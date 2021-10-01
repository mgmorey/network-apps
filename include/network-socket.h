#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network-hints.h"      // Hints
#include "network-fd.h"         // Fd
#include "network-host.h"       // Address, Host, Nullable
#include "network-result.h"     // Result

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Fd, Fd> FdPair;
    typedef std::pair<Fd, Result> SocketResult;
    typedef std::pair<FdPair, Result> SocketpairResult;

    struct Socket :
        public Hints,
        public Host
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Socket& sock);

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
        SocketResult socket(bool t_verbose = false) const;
#ifndef _WIN32
        SocketpairResult socketpair(bool t_verbose = false) const;
#endif

    private:
        static const std::string m_delim;
        static const int m_tab;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Socket& sock);
}

#endif
