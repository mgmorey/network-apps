#include "network/open.h"       // Endpoint, Fd, Hints, OpenHandler,
                                // Result, Sockets, SockAddr,
                                // SocketResult, SocketResults,
                                // get_sockets(), open(), operator<<()

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
    const auto socket_result {get_socket(t_sock, m_verbose)};

    if (std::holds_alternative<Result>(socket_result)) {
        const auto result {std::get<Result>(socket_result)};
        return result;
    }
    else if (std::holds_alternative<Fd>(socket_result)) {
        const auto fd {std::get<Fd>(socket_result)};
        return open(fd, t_sock.address());
    }
    else {
        abort();
    }
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

    if (std::holds_alternative<Result>(sockets_result)) {
        results.push_back(std::get<Result>(sockets_result));
    }
    else if (std::holds_alternative<Sockets>(sockets_result)) {
        const auto sockets {std::get<Sockets>(sockets_result)};
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Open(handler, verbose));
    }
    else {
        abort();
    }

    return results;
}
