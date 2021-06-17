#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network-endpoint.h"   // EndpointResult
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socket()
#include <ws2tcpip.h>   // struct addrinfo, socklen_t
#else
#include <netdb.h>      // struct addrinfo
#include <sys/socket.h> // struct sockaddr, socket(), socklen_t
#endif

#include <string>       // std::string

namespace Network
{
    class Address
    {
    public:
        enum { CONNECT_ERROR = -1 };
        Address();
        Address(const Address& other);
        Address(const struct addrinfo& other);
        ~Address();
        Address& operator=(const Address& other);
        Address& operator=(const struct addrinfo& other);
        bool operator<(const Address& other) const;
        bool operator>(const Address& other) const;
        bool operator==(const Address& other) const;
        Result connect(int fd) const;
        EndpointResult endpoint(int flags = 0) const;
        Hostname hostname(int flags = 0) const;
        Service service(int flags = 0) const;

    private:
        void copy(const struct addrinfo& other);
        const struct sockaddr* data() const;
        socklen_t size() const;

        std::string addr;
    };
}

#endif
