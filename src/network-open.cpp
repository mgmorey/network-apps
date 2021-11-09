#include "network-open.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, open(), operator<<()
#include "network-close.h"      // close()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid()
#include "network-sockets.h"    // get_sockets()

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
    auto socket_result {t_socket.socket(m_verbose)};
    const auto fd {socket_result.first};

    if (!fd) {
        return socket_result;
    }

    const auto open_result {open(fd, t_socket)};

    if (open_result.result()) {
        return SocketResult(close(fd), open_result);
    }

    const auto hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Open::open(Fd t_fd, const Socket& t_socket) const
{
    const auto sock_addr {t_socket.address()};
    return Network::open(m_method, t_fd, sock_addr, m_verbose);
}

Network::Result Network::open(const OpenMethod& method, Fd fd,
                              const SockAddr& sock_addr,
                              bool verbose)
{
    Result result;
    assert(is_valid(sock_addr, verbose));
    const auto addr_ptr {get_pointer(sock_addr)};
    const auto addr_len {get_length(sock_addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << method.second
                  << '('
                  << fd
                  << ", "
                  << sock_addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    reset_last_error();

    if (method.first(fd, addr_ptr, addr_len) == socket_error) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << method.second
            << '('
            << fd
            << ", "
            << sock_addr
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
    const auto [sockets,result] {get_sockets(endpoint, hints, verbose)};

    if (result.result()) {
        Fd fd {fd_null};
        SocketResult socket_result(fd, result);
        results.push_back(socket_result);
    }
    else {
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Open(method, verbose));
    }

    return results;
}
