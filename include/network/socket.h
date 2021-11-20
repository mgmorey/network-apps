#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/fd.h"             // Fd
#include "network/fdarray.h"        // FdArray
#include "network/hints.h"          // Hints
#include "network/host.h"           // Host
#include "network/result.h"         // Result

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::variant

namespace Network
{
    using FdArrayResult = std::variant<FdArray, Result>;
    using FdResult = std::variant<Fd, Result>;

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
        Socket& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Socket& t_sock) const;
        bool operator>(const Socket& t_sock) const;
        bool operator==(const Socket& t_sock) const;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Socket& sock);
    extern FdResult get_socket(const Socket& sock,
                               bool verbose = false);
#ifndef _WIN32
    extern FdArrayResult get_socketpair(const Socket& sock,
                                        bool verbose = false);
#endif
}

#endif
