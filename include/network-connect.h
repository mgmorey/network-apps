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

    enum { connect_error = -1 };

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
    extern SocketResults connect(const Endpoint& endpoint,
                                 const Hints* hints = nullptr,
                                 bool verbose = false);
}

#endif
