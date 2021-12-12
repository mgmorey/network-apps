#include "network/close.h"      // Fd, fd_null, fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

auto Network::close(fd_type fd) -> Network::fd_type
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

auto Network::close(Fd fd) -> Network::Fd
{
    return {close(static_cast<fd_type>(fd))};
}
