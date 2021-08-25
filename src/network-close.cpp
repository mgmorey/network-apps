#include "network-close.h"      // close(), sock_fd_null, sock_fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

void Network::close(sock_fd_type sock_fd)
{
    if (sock_fd == sock_fd_null) {
        return;
    }

#ifdef _WIN32
    ::closesocket(sock_fd);
#else
    ::close(sock_fd);
#endif
}
