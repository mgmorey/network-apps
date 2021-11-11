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
#include <vector>       // std::vector

Network::Open::Open(const OpenMethod& t_method, bool t_verbose) :
    m_method(t_method),
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
    return Network::open(m_method, t_fd, addr, m_verbose);
}

Network::SocketsResult Network::get_sockets(const Network::Endpoint& endpoint,
                                            const Network::Hints* hints,
                                            bool verbose)
{
    return AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}

Network::Result Network::open(const OpenMethod& method, Fd fd,
                              const SockAddr& addr,
                              bool verbose)
{
    Result result;
    assert(is_valid(addr, verbose));
    const auto addr_len {get_length(addr)};
    const auto addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << method.second
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

    if (method.first(fd, addr_ptr, addr_len) == socket_error) {
        const auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << method.second
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

Network::SocketResults Network::open(const OpenMethod& method,
                                     const Endpoint& endpoint,
                                     const Hints* hints,
                                     bool verbose)
{
    SocketResults results;
    const auto [sockets, result] {get_sockets(endpoint, hints, verbose)};

    if (result.result()) {
        const Fd fd {fd_null};
        const SocketResult socket_result(fd, result);
        results.push_back(socket_result);
    }
    else {
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Open(method, verbose));
    }

    return results;
}
