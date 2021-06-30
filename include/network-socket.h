#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network-fd.h"         // fd_type
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<fd_type, Result> SocketResult;
    typedef std::pair<FdPair, Result> SocketPairResult;

    class Socket :
        public addrinfo
    {
    public:
        Socket(int family = 0,
               int socktype = 0,
               int protocol = 0,
               int flags = 0);
        Socket(const Socket& other);
        Socket(const addrinfo& other);
        ~Socket();
        Socket& operator=(const Socket& other);
        Socket& operator=(const addrinfo& other);
        bool operator<(const Socket& other) const;
        bool operator>(const Socket& other) const;
        bool operator==(const Socket& other) const;
        Hostname cname() const;
        SocketResult socket(bool verbose = false) const;
#ifndef _WIN32
        SocketPairResult socketpair(bool verbose = false) const;
#endif

    private:
        static addrinfo defaults(int protocol = 0,
                                 int socktype = 0,
                                 int family = 0,
                                 int flags = 0);
    };
}

#endif
