#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network/family.h"         // family_type
#include "network/types.h"          // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // offsetof()
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Address
    {
    public:
        using port_type = unsigned short;
        using length_type = unsigned short;
        using value_type = SockAddr;

        // cppcheck-suppress noExplicitConstructor
        Address(value_type t_value);  // NOLINT
        Address& operator=(const value_type& t_value);
        bool operator<(const Address& t_address) const;
        bool operator>(const Address& t_address) const;
        bool operator==(const Address& t_address) const;
        operator value_type() const;  // NOLINT
        [[nodiscard]] bool empty() const;
        [[nodiscard]] family_type family() const;
        [[nodiscard]] length_type length() const;
        [[nodiscard]] port_type port() const;
        [[nodiscard]] std::string text() const;

    protected:
        [[nodiscard]] const sockaddr* sa() const;
        [[nodiscard]] value_type sa_data() const;
        [[nodiscard]] family_type sa_family() const;
        [[nodiscard]] length_type sa_length() const;
        [[nodiscard]] std::string sa_text() const;

        [[nodiscard]] const sockaddr_in* sin() const;
        [[nodiscard]] in_addr sin_addr() const;
        [[nodiscard]] family_type sin_family() const;
        [[nodiscard]] length_type sin_length() const;
        [[nodiscard]] port_type sin_port() const;
        [[nodiscard]] std::string sin_text() const;

        [[nodiscard]] const sockaddr_in6* sin6() const;
        [[nodiscard]] in6_addr sin6_addr() const;
        [[nodiscard]] family_type sin6_family() const;
        [[nodiscard]] length_type sin6_length() const;
        [[nodiscard]] port_type sin6_port() const;
        [[nodiscard]] std::string sin6_text() const;

#ifndef _WIN32
        [[nodiscard]] const sockaddr_un* sun() const;
        [[nodiscard]] family_type sun_family() const;
        [[nodiscard]] length_type sun_length() const;
        [[nodiscard]] value_type sun_path() const;
        [[nodiscard]] std::string sun_text() const;
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
