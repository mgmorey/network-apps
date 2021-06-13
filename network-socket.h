#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

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
    class Socket
    {
    public:
        typedef std::pair<int, std::string> SocketResult;
        enum { SOCKET_BAD = -1 };
        Socket();
        Socket(const Socket& other);
        Socket(const struct addrinfo& other);
        Socket(int protocol, int socktype, int family, int flags = 0);
        Socket(int family, int flags = 0);
        ~Socket();
        Socket& operator=(const Socket& other);
        Socket& operator=(const struct addrinfo& ai);
        bool operator<(const Socket& other) const;
        bool operator>(const Socket& other) const;
        bool operator!=(const Socket& other) const;
        bool operator==(const Socket& other) const;
        operator const struct addrinfo&() const;
        Hostname cname() const;
        SocketResult socket() const;

    private:
        void copy(const Socket& other);
        void copy(const struct addrinfo& other);
        void init(int protocol = 0,
                  int socktype = 0,
                  int family = 0,
                  int flags = 0);

        struct addrinfo ai;
    };
}

#endif
