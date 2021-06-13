#include "network-connect.h"    // connect_socket()
#include "network-address.h"    // Address
#include "network-sockets.h"    // Sockets

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

void Network::close_socket(int fd)
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

Network::ConnectResult Network::connect_socket(const Hostname& host,
                                               const Service& service,
                                               const struct addrinfo &hints)
{
    Sockets sockets(get_sockets(host, service, hints));
    Address::ConnectResult connect_result;
    Socket::SocketResult socket_result;
    std::string canonical_name;
    std::string error;
    int fd = -1;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        socket_result = it->socket();
        fd = socket_result.first;

        if (fd == Socket::SOCKET_BAD) {
            continue;
        }

        Address address(*it);
        connect_result = address.connect(fd);

        if (connect_result.first == Address::CONNECT_ERROR) {
            close_socket(fd);
            fd = -1;
        }
        else {
            canonical_name = it->cname();
            break;
        }
    }

    if (fd == Socket::SOCKET_BAD) {
        if (socket_result.first == Socket::SOCKET_BAD) {
            error = socket_result.second;
        }
        else if (connect_result.first == Address::CONNECT_ERROR) {
            error = connect_result.second;
        }

        return ConnectResult(fd, error);
    }

    return ConnectResult(fd, canonical_name);
}
