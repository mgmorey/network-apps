#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network/bytes.h"              // Bytes
#include "network/family-type.h"        // family_type
#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // inet_ntop(), sockaddr_in6
#else
#include <arpa/inet.h>      // inet_ntop()
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
        auto operator=(const value_type& t_value) -> Address&;
        auto operator<(const Address& t_address) const -> bool;
        auto operator>(const Address& t_address) const -> bool;
        auto operator==(const Address& t_address) const -> bool;
        operator value_type() const;  // NOLINT
        [[nodiscard]] auto empty() const -> bool;
        [[nodiscard]] auto family() const -> family_type;
        [[nodiscard]] auto length() const -> length_type;
        [[nodiscard]] auto port() const -> port_type;
        [[nodiscard]] auto text() const -> std::string;

    protected:
        [[nodiscard]] auto sa() const -> const sockaddr*;
        [[nodiscard]] auto sa_data() const -> value_type;
        [[nodiscard]] auto sa_family() const -> family_type;
        [[nodiscard]] auto sa_length() const -> length_type;
        [[nodiscard]] auto sa_text() const -> std::string;

        [[nodiscard]] auto sin() const -> const sockaddr_in*;
        [[nodiscard]] auto sin_addr() const -> in_addr;
        [[nodiscard]] auto sin_port() const -> port_type;
        [[nodiscard]] auto sin_text() const -> std::string;

        [[nodiscard]] auto sin6() const -> const sockaddr_in6*;
        [[nodiscard]] auto sin6_addr() const -> in6_addr;
        [[nodiscard]] auto sin6_port() const -> port_type;
        [[nodiscard]] auto sin6_text() const -> std::string;

#ifndef _WIN32
        [[nodiscard]] auto sun() const -> const sockaddr_un*;
        [[nodiscard]] auto sun_path() const -> value_type;
        [[nodiscard]] auto sun_text() const -> std::string;
#endif

    private:
        value_type m_value;
    };

    extern auto operator<<(std::ostream& os,
                           const Address& address) -> std::ostream&;
}

#endif
