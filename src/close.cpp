#include "network/close.h"      // Fd, close(), fd_null, fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

Network::fd_type Network::close(fd_type fd)
{
    if (fd != fd_null) {
#ifdef _WIN32
        ::closesocket(fd);
#else
        ::close(fd);
#endif
    }

    return fd_null;
}

Network::Fd Network::close(Fd fd)
{
    return Fd(close(static_cast<fd_type>(fd)));
}
