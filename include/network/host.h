#ifndef NETWORK_HOST_H
#define NETWORK_HOST_H

#include "network/bytes.h"              // Bytes
#include "network/hostname.h"           // Hostname

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Host
    {
        static auto get_sockaddr(const addrinfo& ai) -> Bytes;

        Host() = default;
        // cppcheck-suppress noExplicitConstructor
        Host(const addrinfo& t_addrinfo);  // NOLINT
        auto operator=(const addrinfo& t_addrinfo) -> Host&;
        auto operator<(const Host& t_host) const -> bool;
        auto operator>(const Host& t_host) const -> bool;
        auto operator==(const Host& t_host) const -> bool;
        [[nodiscard]] auto address() const -> Bytes;
        [[nodiscard]] auto canonical_name() const -> Hostname;

    private:
        Bytes m_sock_addr;
        Hostname m_canonname;
    };
}

#endif
