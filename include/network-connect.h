#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Address, Endpoint
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult

#include <vector>       // std::vector

namespace Network
{
    class Connect
    {
    public:
        explicit Connect(bool t_verbose);
        SocketResult operator()(const Socket& t_socket) const;
        SocketResult connect(const Socket& t_socket) const;
        Result connect(Fd t_fd, const Socket& t_socket) const;

    private:
        bool m_verbose {false};
    };

    extern Result connect(Fd fd, const Address& address, bool verbose = false);
    extern std::vector<SocketResult> connect(const Endpoint& endpoint,
                                             const Hints* hints = nullptr,
                                             bool verbose = false);
}

#endif
