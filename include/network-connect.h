#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <vector>       // std::vector
#include <utility>      // std::pair

namespace Network
{
    typedef std::vector<SocketResult> SocketResults;

    class Connect
    {
    public:
        Connect(bool t_verbose);
        SocketResult operator()(const Socket& t_socket);

    private:
        bool m_verbose;
    };

    extern SocketResults connect(const Endpoint& endpoint,
                                 const addrinfo& hints,
                                 bool verbose = false);
}

#endif
