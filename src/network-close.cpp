#include "network-close.h"      // Fd, close(), fd_null, fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

Network::Fd Network::close(Fd socket_fd)
{
    auto fd {static_cast<fd_type>(socket_fd)};

    if (fd != fd_null) {
#ifdef _WIN32
        ::closesocket(fd);
#else
        ::close(fd);
#endif
    }

    return Fd(fd_null);
}
