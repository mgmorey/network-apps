#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // Fd, fd_type
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult
#include "network-types.h"      // SockAddr, operator<<()

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::variant
#include <vector>       // std::vector

namespace Network
{
    using Open = int (*)(fd_type, const sockaddr*, socklen_t);
    using OpenBinding = std::pair<Open, std::string>;
    using Sockets = std::vector<Socket>;
    using SocketResults = std::vector<SocketResult>;
    using SocketsResult = std::variant<Sockets, Result>;

    class Transform
    {
    public:
        explicit Transform(const OpenBinding& t_binding, bool t_verbose);
        SocketResult operator()(const Socket& t_socket) const;
        SocketResult open(Fd fd, const SockAddr& addr) const;

    private:
        OpenBinding m_binding {nullptr, ""};
        bool m_verbose {false};
    };

    extern SocketsResult get_sockets(const Endpoint& endp,
                                     const Hints* hints,
                                     bool verbose);
    extern SocketsResult get_sockets(const Hostname& node,
                                     const Service& serv,
                                     const Hints* hints,
                                     bool verbose);
    extern SocketResults open(const OpenBinding& binding,
                              const Endpoint& endp,
                              const Hints* hints,
                              bool verbose);
    extern Result open(const OpenBinding& binding, Fd fd,
                       const SockAddr& addr,
                       bool verbose);
}

#endif
