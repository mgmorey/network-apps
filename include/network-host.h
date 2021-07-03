#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network-address.h"    // Address
#include "network-fd.h"         // sock_fd_type
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

namespace Network
{
    class Host
    {
    public:
        Host(const addrinfo& other);
        Host& operator=(const addrinfo& other);
        bool operator<(const Host& other) const;
        bool operator>(const Host& other) const;
        bool operator==(const Host& other) const;
        operator Address() const;
        Hostname canonical_name() const;
        Result connect(sock_fd_type fd, bool verbose = false) const;

    private:
        Address address;
        Hostname cname;
    };
}

#endif
