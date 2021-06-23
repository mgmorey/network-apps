#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

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
    class Address
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Address& address);

    public:
        enum { connect_error = -1 };
        Address(const struct addrinfo& other);
        Address& operator=(const struct addrinfo& other);
        bool operator<(const Address& other) const;
        bool operator>(const Address& other) const;
        bool operator==(const Address& other) const;
        operator SockAddr() const;
        Hostname canonical_name() const;
        Result connect(int fd) const;

    private:
        SockAddr addr;
        std::string name;
    };
}

#endif
