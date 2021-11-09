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

#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    using OpenFunction = int (*)(fd_type, const sockaddr*, socklen_t);
    using OpenMethod = std::pair<OpenFunction, std::string>;
    using SocketResults = std::vector<SocketResult>;

    class Open
    {
    public:
        explicit Open(OpenMethod t_method, bool t_verbose);
        SocketResult operator()(const Socket& t_socket) const;
        SocketResult open(const Socket& t_socket) const;
        Result open(Fd t_fd, const Socket& t_socket) const;

    private:
        OpenMethod m_method {nullptr, ""};
        bool m_verbose {false};
    };

    extern Result open(OpenMethod method,
                       Fd fd,
                       const SockAddr& sock_addr,
                       bool verbose);
    extern SocketResults open(OpenMethod method,
                              const Endpoint& endpoint,
                              const Hints* hints,
                              bool verbose);
}

#endif
