#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network/endpoint.h"       // Endpoint
#include "network/fd.h"             // Fd, fd_type
#include "network/hints.h"          // Hints
#include "network/result.h"         // Result
#include "network/socket.h"         // FdResult, Socket
#include "network/sockets.h"        // get_sockets()
#include "network/types.h"          // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, socklen_t
#include <ws2tcpip.h>       // socklen_t
#else
#include <netdb.h>          // socklen_t
#include <sys/socket.h>     // sockaddr, socklen_t
#endif

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    using OpenFunction = int (*)(fd_type, const sockaddr*, socklen_t);
    using OpenHandler = std::pair<OpenFunction, std::string>;

    class Open
    {
    public:
        explicit Open(const OpenHandler& t_handler, bool t_verbose);
        [[nodiscard]] FdResult operator()(const Socket& t_sock) const;
        [[nodiscard]] FdResult open(Fd t_fd, const SockAddr& t_addr) const;

    private:
        OpenHandler m_handler {nullptr, ""};
        bool m_verbose {false};
    };

    extern FdResults open(const OpenHandler& handler,
                          const Endpoint& endpoint,
                          const Hints* hints,
                          bool verbose);
    extern Result open(const OpenHandler& handler, Fd fd,
                       const SockAddr& addr,
                       bool verbose);
}

#endif
