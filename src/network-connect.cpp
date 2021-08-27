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
    SocketResult result(t_socket.socket(m_verbose));

    if (result.first != sock_fd_null) {
        const Result connect_result(connect(t_socket, result.first));

        if (connect_result.nonzero()) {
            close(result.first);
            result.first = sock_fd_null;
            result.second = connect_result;
        }
        else {
            result.second = Result(0, t_socket.cname());
        }
    }

    assert(result.first != sock_fd_null ?
           result.second.result() == 0 :
           result.second.result() != 0 &&
           result.second.string() != "");
    return result;
}

Network::Result Network::Connect::connect(const Socket& t_socket,
                                          sock_fd_type t_sock_fd)
{
    const Address address(static_cast<Host>(t_socket));
    return address.connect(t_sock_fd, m_verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const addrinfo& hints,
                                        bool verbose)
{
    SocketResults results;
    const SocketsResult sockets_result(get_sockets(endpoint, hints, verbose));
    const Sockets sockets(sockets_result.first);
    const Result result(sockets_result.second);
    std::transform(sockets.begin(), sockets.end(),
                   std::back_inserter(results),
                   Connect(verbose));
    return results;
}
