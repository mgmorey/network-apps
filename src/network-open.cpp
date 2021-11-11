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

Network::Open::Open(const OpenBinding& t_binding, bool t_verbose) :
    m_binding(t_binding),
    m_verbose(t_verbose)
{
}

Network::SocketResult Network::Open::operator()(const Socket& t_socket) const
{
    return open(t_socket);
}

Network::SocketResult Network::Open::open(const Socket& t_socket) const
{
    const auto [fd, result] {t_socket.socket(m_verbose)};

    if (!fd) {
        return {fd, result};
    }

    const auto open_result {open(fd, t_socket)};

    if (open_result.result()) {
        return {close(fd), open_result};
    }

    const auto hostname {t_socket.canonical_name()};
    return {fd, Result(0, hostname)};
}

Network::Result Network::Open::open(Fd t_fd, const Socket& t_socket) const
{
    const auto addr {t_socket.address()};
    return Network::open(m_binding, t_fd, addr, m_verbose);
}

Network::SocketsResult Network::get_sockets(const Network::Hostname& node,
                                            const Network::Service& serv,
                                            const Network::Hints* hints,
                                            bool verbose)
{
    Sockets sockets;
    const auto result {AddrInfo::insert(node, serv, hints, verbose,
                                        std::back_inserter(sockets))};
    return {sockets, result};
}

Network::SocketsResult Network::get_sockets(const Network::Endpoint& endp,
                                            const Network::Hints* hints,
                                            bool verbose)
{
    SocketsResult sockets_result;
    const auto hostname_result {Network::get_hostname(endp)};

    if (std::holds_alternative<Result>(hostname_result)) {
        sockets_result.second = std::get<Result>(hostname_result);
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

Network::SocketResults Network::open(const OpenBinding& binding,
                                     const Endpoint& endp,
                                     const Hints* hints,
                                     bool verbose)
{
    SocketResults results;
    const auto [sockets, result] {get_sockets(endp, hints, verbose)};

    if (result.result()) {
        const Fd fd {fd_null};
        const SocketResult socket_result(fd, result);
        results.push_back(socket_result);
    }
    else {
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Open(binding, verbose));
    }

    return results;
}
