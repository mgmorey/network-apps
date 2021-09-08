#include "network-close.h"      // close(), sock_fd_null, sock_fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

Network::sock_fd_type Network::close(sock_fd_type sock_fd)
{
    if (sock_fd != sock_fd_null) {
#ifdef _WIN32
        ::closesocket(sock_fd);
#else
        ::close(sock_fd);
#endif
    }

    return sock_fd_null;
}
