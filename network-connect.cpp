#include "network-connect.h"    // Hostname, Result, Service, struct
                                // addrinfo, close_socket(),
                                // connect_socket()
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

Network::Result Network::connect_socket(const Hostname& host,
                                        const Service& service,
                                        const struct addrinfo &hints)
{
    Sockets sockets(get_sockets(host, service, hints));
    std::string canonical_name;
    std::string error;
    int fd = -1;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        Result socket_result(it->socket());
        fd = socket_result.first;

        if (fd == Socket::SOCKET_BAD) {
            continue;
        }

        Address address(*it);
        Result connect_result(address.connect(fd));

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

        return Result(fd, error);
    }

    return Result(fd, canonical_name);
}
