#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // type_fd
#include "network-result.h"     // Result
#include "network-sockets.h"    // Sockets

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <vector>       // std::vector
#include <utility>      // std::pair

namespace Network
{
    typedef std::vector<Result> ConnectDetails;
    typedef std::pair<sock_fd_type, ConnectDetails> ConnectResult;

    class Connector
    {
    public:
        Connector(ConnectDetails& t_connect_details,
                  sock_fd_type& t_sock_fd,
                  bool t_verbose);
        bool operator()(Socket socket);

    private:
        ConnectDetails& m_connect_details;
        sock_fd_type& m_sock_fd;
        bool m_verbose;
    };

    extern ConnectResult connect(const Endpoint& endpoint,
                                 const addrinfo &hints,
                                 bool verbose = false);
    extern ConnectResult connect(const Sockets& sockets,
                                 bool verbose = false);
}

#endif
