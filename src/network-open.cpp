#include "network-open.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, get_sockets(), open(),
                                // operator<<()
#include "network-addrinfo.h"   // AddrInfo
#include "network-close.h"      // close()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid()

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::back_inserter()
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <variant>      // std::get(), std::holds_alternative()
#include <vector>       // std::vector

Network::Transform::Transform(const OpenBinding& t_binding, bool t_verbose) :
    m_binding(t_binding),
    m_verbose(t_verbose)
{
}

Network::SocketResult
Network::Transform::operator()(const Socket& t_socket) const
{
    const auto socket_result {t_socket.socket(m_verbose)};

    if (std::holds_alternative<Result>(socket_result)) {
        const auto result {std::get<Result>(socket_result)};
        return result;
    }
    else if (std::holds_alternative<Fd>(socket_result)) {
        const auto fd {std::get<Fd>(socket_result)};
        return open(fd, t_socket.address());
    }
    else {
        abort();
    }
}

Network::SocketResult
Network::Transform::open(Fd fd, const SockAddr& addr) const
{
    const auto result {Network::open(m_binding, fd, addr, m_verbose)};

    if (result) {
        return result;
    }
    else {
        return fd;
    }
}

Network::SocketsResult Network::get_sockets(const Network::Endpoint& endp,
                                            const Network::Hints* hints,
                                            bool verbose)
{
    SocketsResult sockets_result;
    const auto hostname_result {Network::get_hostname(endp)};

    if (std::holds_alternative<Result>(hostname_result)) {
        return std::get<Result>(hostname_result);
    }
    else if (std::holds_alternative<Hostname>(hostname_result)) {
        const auto host {std::get<Network::Hostname>(hostname_result)};
        sockets_result = get_sockets(host, endp.second, hints, verbose);
    }
    else {
        abort();
    }

    return sockets_result;
}

Network::SocketsResult Network::get_sockets(const Network::Hostname& node,
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

Network::SocketResults Network::open(const OpenBinding& binding,
                                     const Endpoint& endp,
                                     const Hints* hints,
                                     bool verbose)
{
    SocketResults results;
    const auto sockets_result {get_sockets(endp, hints, verbose)};

    if (std::holds_alternative<Result>(sockets_result)) {
        results.push_back(std::get<Result>(sockets_result));
    }
    else if (std::holds_alternative<Sockets>(sockets_result)) {
        const auto sockets {std::get<Sockets>(sockets_result)};
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Transform(binding, verbose));
    }
    else {
        abort();
    }

    return results;
}

Network::Result Network::open(const OpenBinding& binding, Fd fd,
                              const SockAddr& addr,
                              bool verbose)
{
    Result result;
    assert(is_valid(addr, verbose));
    const auto addr_len {get_length(addr)};
    const auto addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << binding.second
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

    if (binding.first(fd, addr_ptr, addr_len) == socket_error) {
        const auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << binding.second
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
