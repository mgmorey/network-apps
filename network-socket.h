#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

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
    class Socket
    {
        friend std::ostream& operator<<(std::ostream&, const Socket&);

    public:
        enum { socket_bad = -1 };
        Socket();
        Socket(int family, int flags = 0);
        Socket(int protocol, int socktype, int family, int flags = 0);
        Socket(const Socket& other);
        Socket(const struct addrinfo& other);
        ~Socket();
        Socket& operator=(const Socket& other);
        Socket& operator=(const struct addrinfo& other);
        bool operator<(const Socket& other) const;
        bool operator>(const Socket& other) const;
        bool operator==(const Socket& other) const;
        operator const struct addrinfo&() const;
        Hostname cname() const;
        Result socket() const;

    private:
        static struct addrinfo defaults(int protocol = 0,
                                        int socktype = 0,
                                        int family = 0,
                                        int flags = 0);

        struct addrinfo ai;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Socket& socket);
}

#endif
