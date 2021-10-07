#include "network-connect.h"    // Endpoint, Result, SocketResult,
                                // SocketResults,
#include "network-addrinfo.h"   // AddrInfo
#include "network-close.h"      // close()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-fd.h"         // Fd, fd_null, fd_type
#include "network-host.h"       // get_sockaddr()
#include "network-sockaddr.h"   // get_length(), get_pointer()
#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname, SockAddr

#ifdef _WIN32
#include <winsock2.h>   // connect()
#else
#include <sys/socket.h> // connect()
#endif

#include <algorithm>    // std::transform()
#include <cassert>      // assert()
#include <iterator>     // std::back_inserter()
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair
#include <vector>       // std::vector

typedef std::vector<Network::Socket> Sockets;
typedef std::pair<Sockets, Network::Result> SocketsResult;

static SocketsResult get_sockets(const Network::Endpoint& endpoint,
                                 const Network::Hints* hints,
                                 bool verbose)
{
    return Network::AddrInfo::get<SocketsResult>(endpoint, hints, verbose);
}

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

    Network::Hostname hostname {t_socket.canonical_name()};
    socket_result.second = Result(0, hostname);
    return socket_result;
}

Network::Result Network::Connect::connect(Fd t_fd, const Socket& t_socket) const
{
    return Network::connect(t_fd, t_socket.address(), m_verbose);
}

Network::Result Network::connect(Fd fd, const Address& address, bool verbose)
{
    assert(!address.empty());

    if (verbose) {
        std::cerr << "Trying "
                  << address
                  << std::endl;
    }

    std::string message;
    SockAddr addr {address};
    auto error {reset_last_error()};
    auto addr_ptr {get_pointer(addr)};
    std::size_t addr_len {get_length(addr)};
    std::size_t addr_size {get_max_size(addr)};
    const auto code {::connect(fd, addr_ptr, addr_len ? addr_len : addr_size)};

    if (code == connect_error) {
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
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    return {error, message};
}

Network::SocketResults Network::connect(const Endpoint& endpoint,
                                        const Hints* hints,
                                        bool verbose)
{
    SocketResults results;
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
