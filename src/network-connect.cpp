#include "network-connect.h"    // ConnectResult, ConnectResults,
                                // Endpoint, Hostname, Result,
                                // Sockets, SocketsResult, struct
                                // addrinfo, sock_fd_type
#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_null
#include "network-host.h"       // Host

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
        const Address address(static_cast<Host>(t_socket));
        const Result connect_result(address.connect(result.first, m_verbose));

        if (connect_result.nonzero()) {
            close(result.first);
            result.first = sock_fd_null;
            result.second = connect_result;
        }
        else {
            result.second = Result(0, t_socket.cname());
        }
    }

    assert(result.first == sock_fd_null ?
           result.second.result() != 0 && result.second.string() != "":
           result.second.result() == 0);
    return result;
}

Network::SocketResults Network::connect(const Sockets& sockets,
                                        bool verbose)
{
    SocketResults results;
    std::transform(sockets.begin(), sockets.end(),
                   std::back_inserter(results),
                   Connect(verbose));
    return results;
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const addrinfo& hints,
                                        bool verbose)
{
    SocketResults results;
    const SocketsResult sockets_result(get_sockets(endpoint, hints, verbose));
    const Sockets sockets(sockets_result.first);
    const Result result(sockets_result.second);

    if (!result.nonzero()) {
        results = connect(sockets, verbose);
    }

    return results;
}
