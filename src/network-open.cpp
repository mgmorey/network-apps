#include "network-open.h"       // Endpoint, Fd, Hints, Result,
                                // Socket, SockAddr, SocketResult,
                                // fd_null, operator<<()
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

Network::Open::Open(open_method_type t_method,
                    std::string t_name,
                    bool t_verbose) :
    m_method(t_method),
    m_name(t_name),
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

    if (open_result.result() != 0) {
        return SocketResult(close(fd), open_result);
    }

    const auto hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Open::open(Fd t_fd, const Socket& t_socket) const
{
    return Network::open(t_fd, t_socket.address(), m_method, m_name, m_verbose);
}

Network::Result Network::open(Fd fd,
                              const SockAddr& sock_addr,
                              const open_method_type method,
                              const std::string& name,
                              bool verbose)
{
    Result result;
    assert(is_valid(sock_addr, verbose));
    const auto addr_ptr {get_pointer(sock_addr)};
    const auto addr_len {get_length(sock_addr)};
    // cppcheck-suppress variableScope
    auto error {reset_last_error()};

    if (verbose) {
        std::cerr << "Calling "
                  << name
                  << '('
                  << fd
                  << ", "
                  << sock_addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    if (method(fd, addr_ptr, addr_len) == socket_error) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << name
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

std::vector<Network::SocketResult> Network::open(const Endpoint& endpoint,
                                                 const Hints* hints,
                                                 const open_method_type method,
                                                 const std::string& name,
                                                 bool verbose)
{
    std::vector<SocketResult> results;
    const auto sockets_result {get_sockets(endpoint, hints, verbose)};
    const auto sockets {sockets_result.first};
    const auto result {sockets_result.second};

    if (result.result() != 0) {
        Fd fd {fd_null};
        SocketResult socket_result(fd, result);
        results.push_back(socket_result);
    }
    else {
        std::transform(sockets.begin(), sockets.end(),
                       std::back_inserter(results),
                       Open(method, name, verbose));
    }

    return results;
}
