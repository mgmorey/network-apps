#include "network-bind.h"       // Address, Endpoint, Fd, Hints,
                                // Result, Socket, SocketResult,
                                // fd_null
#include "network-addrinfo.h"   // AddrInfo
#include "network-close.h"      // close()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer()

#ifdef _WIN32
#include <winsock2.h>   // bind()
#else
#include <sys/socket.h> // bind()
#endif

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iterator>     // std::back_inserter()
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <vector>       // std::vector

using Sockets = std::vector<Network::Socket>;
using SocketsResult = std::pair<Sockets, Network::Result>;

static SocketsResult get_sockets(const Network::Endpoint& endpoint,
                                 const Network::Hints* hints,
                                 bool verbose)
{
    return Network::AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}

Network::Bind::Bind(bool t_verbose) :
    m_verbose(t_verbose)
{
}

Network::SocketResult Network::Bind::operator()(const Socket& t_socket) const
{
    return bind(t_socket);
}

Network::SocketResult Network::Bind::bind(const Socket& t_socket) const
{
    auto socket_result {t_socket.socket(m_verbose)};
    const auto fd {socket_result.first};

    if (!fd) {
        return socket_result;
    }

    const auto bind_result {bind(fd, t_socket)};

    if (bind_result.result() != 0) {
        return SocketResult(close(fd), bind_result);
    }

    const auto hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Bind::bind(Fd t_fd, const Socket& t_socket) const
{
    return Network::bind(t_fd, t_socket.address(), m_verbose);
}

Network::Result Network::bind(Fd fd, const Address& address, bool verbose)
{
    assert(!address.empty());

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
    const auto code {::bind(fd, addr_ptr, addr_len)};

    if (code == bind_error) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to bind("
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

std::vector<Network::SocketResult> Network::bind(const Endpoint& endpoint,
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
                       Bind(verbose));
    }

    return results;
}
