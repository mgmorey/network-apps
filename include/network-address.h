#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network-endpoint.h"   // EndpointResult
#include "network-fd.h"         // Fd
#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in, struct
                        // sockaddr_in6
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#include <sys/un.h>     // struct sockaddr_un
#endif

#include <cstddef>      // std::byte
#include <ostream>      // std::ostream
#include <vector>       // std::vector

namespace Network
{
    typedef std::vector<std::byte> Bytes;

    class Address
    {
    public:
#ifdef _WIN32
        typedef unsigned short family_type;
        typedef unsigned short port_type;
        typedef std::size_t sock_len_type;
#else
        typedef sa_family_t family_type;
        typedef in_port_t port_type;
        typedef socklen_t sock_len_type;
#endif
        enum { connect_error = -1 };

        Address(const sockaddr* t_sockaddr = nullptr,
                sock_len_type t_socklen = 0);
        bool operator<(const Address& t_address) const;
        bool operator>(const Address& t_address) const;
        bool operator==(const Address& t_address) const;
        Result connect(Fd t_fd,
                       bool t_verbose = false) const;
        bool empty() const;
        family_type family() const;
        port_type port() const;
        std::string text() const;
        EndpointResult to_endpoint(int t_flags,
                                   bool t_verbose = false) const;
        EndpointResult to_endpoint(bool t_numeric = false,
                                   bool t_verbose = false) const;

    protected:
        const sockaddr* addr() const;
        sock_len_type addrlen() const;

        operator const sockaddr&() const;
        Bytes sa_data() const;
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

    private:
        Bytes m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Address& address);
}

#endif
