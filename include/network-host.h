#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-result.h"     // Result
#include "network-sockaddr.h"   // SockAddr
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr
#include <ws2tcpip.h>   // struct addrinfo, socklen_t
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <string>       // std::string

namespace Network
{
    class Host
    {
    public:
        enum { connect_error = -1 };
        Host(const addrinfo& other);
        Host& operator=(const addrinfo& other);
        bool operator<(const Host& other) const;
        bool operator>(const Host& other) const;
        bool operator==(const Host& other) const;
        const SockAddr& address() const;
        Hostname canonical_name() const;
        Result connect(int fd) const;

    private:
        SockAddr addr;
        Hostname name;
    };
}

#endif
