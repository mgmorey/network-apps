#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Hostname, Result, Service, struct
                                // addrinfo, close_socket(),
                                // connect_socket()
#include "network-address.h"    // Address
#include "network-sockets.h"    // Sockets

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

void Network::close(int fd)
{
    if (fd == -1) {
        return;
    }

#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}

Network::ConnectResult Network::connect(const Hostname& host,
                                        const Service& service,
                                        const struct addrinfo &hints)
{
    SocketsResult result(get_sockets(host, service, hints));
    Sockets sockets(result.first);
    Result socks_result(result.second);
    int code = socks_result.first;
    int fd = Socket::socket_bad;
    ConnectDetails details;

    if (code != 0) {
        details.push_back(socks_result.second);
        return ConnectResult(fd, details);
    }

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        Result sock_result(it->socket());
        fd = sock_result.first;

        if (fd == Socket::socket_bad) {
            details.push_back(sock_result.second);
            continue;
        }

        Result conn_result(Address(*it).connect(fd));

        if (conn_result.first == Address::connect_error) {
            close(fd);
            fd = Socket::socket_bad;
            details.push_back(conn_result.second);
        }
        else {
            details.push_back(it->cname());
            break;
        }
    }

    return ConnectResult(fd, details);
}
