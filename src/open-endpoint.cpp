#include "network/open.h"       // Endpoint, Fd, Hints, OpenHandler,
                                // Result, SockAddr, SocketResult,
                                // SocketResults, Sockets,
                                // get_sockets(), open(), operator<<()
#include "network/overload.h"   // Overload

#include <algorithm>    // std::transform()
#include <iterator>     // std::back_inserter()
#include <variant>      // std::get(), std::holds_alternative()

Network::Open::Open(const OpenHandler& t_handler, bool t_verbose) :
    m_handler(t_handler),
    m_verbose(t_verbose)
{
}

Network::SocketResult
Network::Open::operator()(const Socket& t_sock) const
{
    auto socket_result {get_socket(t_sock, m_verbose)};
    std::visit(Overload {
            [&](Fd fd) {
                socket_result = open(fd, t_sock.address());
            },
            [&](const Result& result) {
                socket_result = result;
            }
        }, socket_result);
    return socket_result;
}

Network::SocketResult
Network::Open::open(Fd t_fd, const SockAddr& t_addr) const
{
    const auto result {Network::open(m_handler, t_fd, t_addr, m_verbose)};

    if (result) {
        return result;
    }
    else {
        return t_fd;
    }
}

Network::SocketResults
Network::open(const OpenHandler& handler,
              const Endpoint& endpoint,
              const Hints* hints,
              bool verbose)
{
    SocketResults results;
    const auto sockets_result {get_sockets(endpoint, hints, verbose)};
    std::visit(Overload {
            [&](const Sockets& sockets) {
                std::transform(sockets.begin(), sockets.end(),
                               std::back_inserter(results),
                               Open(handler, verbose));
            },
            [&](const Result& result) {
                results.push_back(result);
            }
        }, sockets_result);
    return results;
}
