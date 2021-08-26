#include "network-connect.h"    // ConnectResult, ConnectResults,
                                // Endpoint, Hostname, Result,
                                // Sockets, SocketsResult, struct
                                // addrinfo, sock_fd_type
#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_null
#include "network-host.h"       // Host

#include <algorithm>    // std::find_if()

Network::Connect::Connect(ConnectResults& t_results,
                          sock_fd_type& t_sock_fd,
                          bool t_verbose) :
    m_results(t_results),
    m_sock_fd(t_sock_fd),
    m_verbose(t_verbose)
{
}

bool Network::Connect::operator()(const Socket& t_socket)
{
    const SocketResult socket_result(t_socket.socket(m_verbose));
    m_sock_fd = socket_result.first;

    if (m_sock_fd == sock_fd_null) {
        m_results.push_back(socket_result.second);
        return false;
    }

    const Address address(static_cast<Host>(t_socket));
    const Result result(address.connect(m_sock_fd, m_verbose));

    if (result.result()) {
        close(m_sock_fd);
        m_sock_fd = sock_fd_null;
        m_results.push_back(result);
        return false;
    }

    m_results.push_back(Result(0, t_socket.cname()));
    return true;
}

Network::ConnectResult Network::connect(const Sockets& sockets,
                                        bool verbose)
{
    ConnectResults results;
    sock_fd_type sock_fd = sock_fd_null;
    static_cast<void>(std::find_if(sockets.begin(), sockets.end(),
                                   Connect(results, sock_fd, verbose)));
    return ConnectResult(sock_fd, results);
}

Network::ConnectResult Network::connect(const Endpoint& endpoint,
                                        const addrinfo& hints,
                                        bool verbose)
{
    const SocketsResult sockets_result(get_sockets(endpoint, hints, verbose));
    const Sockets sockets(sockets_result.first);
    const Result result(sockets_result.second);

    if (result.nonzero()) {
        ConnectResults results;
        results.push_back(result);
        return ConnectResult(sock_fd_null, results);
    }

    return connect(sockets, verbose);
}
