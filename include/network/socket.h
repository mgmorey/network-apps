#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/hints.h"          // Hints
#include "network/host.h"           // Host

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Socket :
        public Hints,
        public Host
    {
        explicit Socket(int t_family = 0,
                        int t_socktype = 0,
                        int t_protocol = 0,
                        int t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Socket(const addrinfo& t_addrinfo);  // NOLINT
        auto operator=(const addrinfo& t_addrinfo) -> Socket&;
        auto operator<(const Socket& t_sock) const -> bool;
        auto operator>(const Socket& t_sock) const -> bool;
        auto operator==(const Socket& t_sock) const -> bool;
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) -> std::ostream&;
}

#endif
