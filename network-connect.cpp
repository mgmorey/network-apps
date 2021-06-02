#include "network-connect.h"
#include "network-address.h"
#include "network-sockets.h"
#include "network-socket.h"

#ifdef _WIN32
#include <winsock2.h>		// closesocket(), connect(),
                                // gethostname(), socket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, connect(), socket(),
                                // socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cassert>		// assert()
#include <cerrno>		// errno
#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
#include <iostream>		// std::cerr, std::endl

void close_socket(int fd)
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

ConnectResult connect_socket(const std::string& host,
                             const std::string& service,
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
