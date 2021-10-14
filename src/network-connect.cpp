#include "network-connect.h"    // Address, Endpoint, Fd, Hints,
                                // Result, Socket, SocketResult,
                                // fd_null
#include "network-close.h"      // close()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid()
#include "network-sockets.h"    // get_sockets()

#ifdef _WIN32
#include <winsock2.h>   // connect()
#else
#include <sys/socket.h> // connect()
#endif

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::back_inserter()
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <vector>       // std::vector

Network::Connect::Connect(bool t_verbose) :
    m_verbose(t_verbose)
{
}

Network::SocketResult Network::Connect::operator()(const Socket& t_socket) const
{
    return connect(t_socket);
}

Network::SocketResult Network::Connect::connect(const Socket& t_socket) const
{
    auto socket_result {t_socket.socket(m_verbose)};
    const auto fd {socket_result.first};

    if (!fd) {
        return socket_result;
    }

    const auto connect_result {connect(fd, t_socket)};

    if (connect_result.result() != 0) {
        return SocketResult(close(fd), connect_result);
    }

    const auto hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Connect::connect(Fd t_fd, const Socket& t_socket) const
{
    return Network::connect(t_fd, t_socket.address(), m_verbose);
}

Network::Result Network::connect(Fd fd, const Address& address, bool verbose)
{
    assert(is_valid(address, verbose));

    if (verbose) {
        std::cerr << "Trying "
                  << address
                  << std::endl;
    }

    Result result;
    const SockAddr sock_addr {address};
    const auto addr_ptr {get_pointer(sock_addr)};
    const auto addr_len {get_length(sock_addr)};
    // cppcheck-suppress variableScope
    auto error {reset_last_error()};

    if (verbose) {
        std::cerr << "Calling connect("
                  << fd
                  << ", "
                  << address
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    if (::connect(fd, addr_ptr, addr_len) == socket_error) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to connect("
            << fd
            << ", "
            << address
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }

    return result;
}

std::vector<Network::SocketResult> Network::connect(const Endpoint& endpoint,
                                                    const Hints* hints,
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
                       Connect(verbose));
    }

    return results;
}
