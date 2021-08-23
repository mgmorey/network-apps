#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network-endpoint.h"   // EndpointResult
#include "network-fd.h"         // sock_fd_type
#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in,
                        // struct sockaddr_in6, SOCKET_ERROR
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#include <sys/un.h>     // struct sockaddr_un
#endif

#include <string>       // std::string

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	(-1)
#endif

namespace Network
{
    class Address
    {
    public:
#ifdef _WIN32
        typedef unsigned short family_type;
        typedef unsigned short port_type;
#else
        typedef sa_family_t family_type;
        typedef in_port_t port_type;
#endif
        enum { connect_error = SOCKET_ERROR };

        Address(const sockaddr* addr, socklen_t addrlen);
        bool operator<(const Address& other) const;
        bool operator>(const Address& other) const;
        bool operator==(const Address& other) const;
        Result connect(sock_fd_type fd, bool verbose = false) const;
        bool empty() const;
        family_type family() const;
        port_type port() const;
        std::string text() const;
        EndpointResult to_endpoint(int flags,
                                   bool verbose = false) const;
        EndpointResult to_endpoint(bool numeric = false,
                                   bool verbose = false) const;

    private:
        const sockaddr* addr() const;
        socklen_t addrlen() const;

        operator const sockaddr&() const;
        std::string sa_data() const;
        family_type sa_family() const;
        std::string sa_text() const;

        operator const sockaddr_in&() const;
        in_addr sin_addr() const;
        family_type sin_family() const;
        port_type sin_port() const;
        std::string sin_text() const;

        operator const sockaddr_in6&() const;
        in6_addr sin6_addr() const;
        family_type sin6_family() const;
        port_type sin6_port() const;
        std::string sin6_text() const;

#ifndef _WIN32
        operator const sockaddr_un&() const;
        family_type sun_family() const;
        std::string sun_path() const;
        std::string sun_text() const;
#endif

        std::string value;
    };
}

#endif
