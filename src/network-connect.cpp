#include "network-connect.h"    // Endpoint, Result, SocketResult,
                                // SocketResults,
#include "network-addrinfo.h"   // AddrInfo
#include "network-close.h"      // close()
#include "network-fd.h"         // SocketFd, fd_null, fd_type
#include "network-hints.h"      // Hints

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <utility>      // std::pair
#include <vector>       // std::vector

typedef std::vector<Network::Socket> Sockets;
typedef std::pair<Sockets, Network::Result> SocketsResult;

static SocketsResult get_sockets(const Network::Endpoint& endpoint,
                                 const Network::Hints* hints,
                                 bool verbose)
{
    return Network::AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}

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
    auto socket_result {t_socket.socket(m_verbose)};
    const auto socket_fd {socket_result.first};

    if (!socket_fd) {
        return socket_result;
    }

    const auto connect_result {connect(t_socket, socket_fd)};

    if (connect_result.result() != 0) {
        return SocketResult(close(socket_fd), connect_result);
    }

    Network::Hostname hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Connect::connect(const Socket& t_socket,
                                          SocketFd t_socket_fd)
{
    return t_socket.address().connect(t_socket_fd, m_verbose);
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const Hints* hints,
                                        bool verbose)
{
    SocketResults results;
    const auto sockets_result {get_sockets(endpoint, hints, verbose)};
    const auto sockets {sockets_result.first};
    const auto result {sockets_result.second};

    if (result.result() != 0) {
        SocketFd socket_fd {fd_null};
        SocketResult socket_result(socket_fd, result);
        results.push_back(socket_result);
    }
    else {
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Connect(verbose));
    }

    return results;
}
