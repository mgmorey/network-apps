#include "network/open.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, get_sockets(), open(),
                                // operator<<()
#include "network/addrinfo.h"   // AddrInfo
#include "network/close.h"      // close()
#include "network/error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network/sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid()

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::back_inserter()
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <variant>      // std::get(), std::holds_alternative()
#include <vector>       // std::vector

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

Network::SocketsResult
Network::get_sockets(const Network::Endpoint& endpoint,
                     const Network::Hints* hints,
                     bool verbose)
{
    SocketsResult sockets_result;
    const auto hostname_result {Network::get_hostname(endpoint.first)};

    if (std::holds_alternative<Result>(hostname_result)) {
        return std::get<Result>(hostname_result);
    }
    else if (std::holds_alternative<Hostname>(hostname_result)) {
        const auto host {std::get<Network::Hostname>(hostname_result)};
        sockets_result = get_sockets(host, endpoint.second, hints, verbose);
    }
    else {
        abort();
    }

    return sockets_result;
}

Network::SocketsResult
Network::get_sockets(const Network::Hostname& node,
                     const Network::Service& serv,
                     const Network::Hints* hints,
                     bool verbose)
{
    Sockets sockets;
    const auto result {AddrInfo::insert(node, serv, hints, verbose,
                                        std::back_inserter(sockets))};

    if (result) {
        return result;
    }

    return sockets;
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

Network::Result
Network::open(const OpenHandler& handler, Fd fd,
              const SockAddr& addr,
              bool verbose)
{
    Result result;
    assert(is_valid(addr, verbose));
    const auto addr_len {get_length(addr)};
    const auto addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    reset_last_error();

    if (handler.first(fd, addr_ptr, addr_len) == socket_error) {
        const auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << addr
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }

    return result;
}
