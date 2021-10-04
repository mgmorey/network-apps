#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network-fd.h"         // Fd
#include "network-result.h"     // Result
#include "network-types.h"      // SockAddr

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in, struct
                        // sockaddr_in6
#include <ws2tcpip.h>   // socklen_t
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr, socklen_t
#include <sys/un.h>     // struct sockaddr_un
#endif

#include <cstddef>      // std::byte, std::size_t
#include <ostream>      // std::ostream
#include <string>       // std::string

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
        typedef SockAddr value_type;

        Address();
        Address(const value_type& t_value);
        Address& operator=(const value_type& t_value);
        bool operator<(const Address& t_address) const;
        bool operator>(const Address& t_address) const;
        bool operator==(const Address& t_address) const;
        operator value_type() const;
        bool empty() const;
        family_type family() const;
        port_type port() const;
        std::string text() const;

    protected:
        const sockaddr& sa() const;
        value_type sa_data() const;
        family_type sa_family() const;
        std::string sa_text() const;

        const sockaddr_in& sin() const;
        in_addr sin_addr() const;
        family_type sin_family() const;
        port_type sin_port() const;
        std::string sin_text() const;

        const sockaddr_in6& sin6() const;
        in6_addr sin6_addr() const;
        family_type sin6_family() const;
        port_type sin6_port() const;
        std::string sin6_text() const;

#ifndef _WIN32
        const sockaddr_un& sun() const;
        family_type sun_family() const;
        value_type sun_path() const;
        std::string sun_text() const;
#endif

    private:
        static constexpr auto m_sa_data_offset {
            offsetof(sockaddr, sa_data)
        };
#ifndef _WIN32
        static constexpr auto m_sun_path_offset {
            offsetof(sockaddr_un, sun_path)
        };
#endif

        value_type m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Address& address);
}

#endif
