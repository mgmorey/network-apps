#ifndef NETWORK_BIND_H
#define NETWORK_BIND_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult
#include "network-types.h"      // SockAddr, operator<<()

#include <vector>       // std::vector

namespace Network
{
    class Bind
    {
    public:
        explicit Bind(bool t_verbose);
        SocketResult operator()(const Socket& t_socket) const;
        SocketResult bind(const Socket& t_socket) const;
        Result bind(Fd t_fd, const Socket& t_socket) const;

    private:
        bool m_verbose {false};
    };

    extern Result bind(Fd fd, const SockAddr& sock_addr, bool verbose = false);
    extern std::vector<SocketResult> bind(const Endpoint& endpoint,
                                          const Hints* hints = nullptr,
                                          bool verbose = false);
}

#endif
