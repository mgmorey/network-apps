#include "network-connect.h"    // Endpoint, Result, SocketResult,
                                // SocketResults,
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_null
#include "network-hints.h"      // Hints
#include "network-sockets.h"    // get_sockets()

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()

Network::Connect::Connect(bool t_verbose) :
    m_verbose(t_verbose)
{
}

Network::SocketResult Network::Connect::operator()(const Socket& t_socket)
{
    return connect(t_socket);
}

Network::SocketResult Network::Connect::connect(const Socket& t_socket)
{
    auto socket_result(t_socket.socket(m_verbose));
    const auto sock_fd = socket_result.first;

    if (sock_fd == sock_fd_null) {
        return socket_result;
    }

    const auto connect_result(connect(t_socket, sock_fd));

    if (connect_result.result() != 0) {
        return SocketResult(close(sock_fd), connect_result);
    }

    socket_result.second = Result(0, t_socket.canonical_name());
    return socket_result;
}

Network::Result Network::Connect::connect(const Socket& t_socket,
                                          sock_fd_type t_sock_fd)
{
    return t_socket.address().connect(t_sock_fd, m_verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const Hints& hints,
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
