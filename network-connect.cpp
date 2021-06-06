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
    std::string canonical_name;
    int fd = -1;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        fd = it->socket();

        if (fd == -1) {
            continue;
        }

        Address address(*it);

        if (address.connect(fd) == -1) {
            close_socket(fd);
            fd = -1;
        }
        else {
            canonical_name = it->cname();
            break;
        }
    }

    return ConnectResult(fd, canonical_name);
}
