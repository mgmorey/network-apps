#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Hostname, Result, Service, Sockets,
                                // SocketsResult, struct addrinfo,
                                // connect()
#include "network-addrinfo.h"   // operator<<()
#include "network-close.h"      // close()
#include "network-endpoint.h"   // Endpoint
#include "network-fd.h"         // sock_fd_null, sock_fd_type
#include "network-host.h"       // Host

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

#include <algorithm>    // std::find_if()
#include <iostream>     // std::cerr, std::endl

Network::Connector::Connector(ConnectDetails& t_connect_details,
                              sock_fd_type& t_sock_fd,
                              bool t_verbose) :
    m_connect_details(t_connect_details),
    m_sock_fd(t_sock_fd),
    m_verbose(t_verbose)
{
}

bool Network::Connector::operator()(Socket socket)
{
    const SocketResult socket_result(socket.socket(m_verbose));
    m_sock_fd = socket_result.first;

    if (m_sock_fd == sock_fd_null) {
        m_connect_details.push_back(socket_result.second);
        return false;
    }

    const Address address(static_cast<Host>(socket));
    const Result connect_result(address.connect(m_sock_fd, m_verbose));

    if (connect_result.result() == Address::connect_error) {
        close(m_sock_fd);
        m_sock_fd = sock_fd_null;
        m_connect_details.push_back(connect_result);
        return false;
    }

    const Result result(0, socket.cname());
    m_connect_details.push_back(result);
    return true;
}

Network::ConnectResult Network::connect(const Endpoint& endpoint,
                                        const addrinfo &hints,
                                        bool verbose)
{
    const SocketsResult sockets_result(get_sockets(endpoint, hints, verbose));
    const Sockets sockets(sockets_result.first);
    const Result result(sockets_result.second);

    if (result.nonzero()) {
        ConnectDetails connect_details;
        connect_details.push_back(result);
        return ConnectResult(sock_fd_null, connect_details);
    }

    return connect(sockets, verbose);
}

Network::ConnectResult Network::connect(const Sockets& sockets,
                                        bool verbose)
{
    ConnectDetails connect_details;
    sock_fd_type sock_fd = sock_fd_null;
    Connector connector(connect_details, sock_fd, verbose);
    Sockets::const_iterator socket =
        std::find_if(sockets.begin(), sockets.end(), connector);
    static_cast<void>(socket);
    return ConnectResult(sock_fd, connect_details);
}
