#include "network-connect.h"    // Endpoint, Result, SocketResults,
                                // SocketsResult
#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_null
#include "network-host.h"       // Host
#include "network-sockets.h"    // Sockets

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iterator>     // std::back_inserter()

Network::Connect::Connect(bool t_verbose) :
    m_verbose(t_verbose)
{
}

Network::SocketResult Network::Connect::operator()(const Socket& t_socket)
{
    auto socket_result(t_socket.socket(m_verbose));

    if (socket_result.first == sock_fd_null) {
        return socket_result;
    }

    const auto connect_result(connect(t_socket, socket_result.first));

    if (connect_result.nonzero()) {
        close(socket_result.first);
        return SocketResult(sock_fd_null, connect_result);
    }

    socket_result.second = Result(0, t_socket.cname());
    return socket_result;
}

Network::Result Network::Connect::connect(const Socket& t_socket,
                                          sock_fd_type t_sock_fd)
{
    const auto address(static_cast<Address>(static_cast<Host>(t_socket)));
    return address.connect(t_sock_fd, m_verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const addrinfo& hints,
                                        bool verbose)
{
    SocketResults results;
    const auto sockets_result(get_sockets(endpoint, hints, verbose));
    const auto sockets(sockets_result.first);
    const auto result(sockets_result.second);
    std::transform(sockets.begin(), sockets.end(),
                   std::back_inserter(results),
                   Connect(verbose));
    return results;
}
