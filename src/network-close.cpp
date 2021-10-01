#include "network-close.h"      // SocketFd, close(), fd_null, fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

Network::SocketFd Network::close(SocketFd socket_fd)
{
    auto fd {static_cast<fd_type>(socket_fd)};

    if (fd != fd_null) {
#ifdef _WIN32
        ::closesocket(fd);
#else
        ::close(fd);
#endif
    }

    return SocketFd(fd_null);
}
