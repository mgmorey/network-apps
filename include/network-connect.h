#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // SocketResult

#include <vector>       // std::vector
#include <utility>      // std::pair

namespace Network
{
    typedef std::vector<SocketResult> SocketResults;

    class Connect
    {
    public:
        explicit Connect(bool t_verbose);
        SocketResult operator()(const Socket& t_socket);
        SocketResult connect(const Socket& t_socket);
        Result connect(const Socket& t_socket,
                       sock_fd_type t_sock_fd);

    private:
        bool m_verbose;
    };

    extern SocketResults connect(const Endpoint& endpoint,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
