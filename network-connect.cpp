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

Network::ConnectResult Network::connect(const Hostname& hostname,
                                        const Service& service,
                                        const struct addrinfo &hints)
{
    std::string error;
    Sockets sockets(get_sockets(error, hostname, service, hints));
    ConnectDetails details;
    int fd = -1;

    if (!error.empty()) {
        details.push_back(error);
        return ConnectResult(fd, details);
    }

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        Result socket_result(it->socket());
        fd = socket_result.first;

        if (fd == Socket::SOCKET_BAD) {
            details.push_back(socket_result.second);
            continue;
        }

        Address address(*it);
        Result connect_result(address.connect(fd));

        if (connect_result.first == Address::CONNECT_ERROR) {
            close(fd);
            fd = Socket::SOCKET_BAD;
            details.push_back(connect_result.second);
        }
        else {
            details.push_back(it->cname());
            break;
        }
    }

    return ConnectResult(fd, details);
}
