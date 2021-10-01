#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketResult

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
                       Fd t_socket_fd);

    private:
        bool m_verbose {false};
    };

    extern SocketResults connect(const Endpoint& endpoint,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
