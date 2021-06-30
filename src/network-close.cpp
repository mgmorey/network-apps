#include "network-close.h"      // close(), fd_null, fd_type

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

void Network::close(fd_type fd)
{
    if (fd == fd_null) {
        return;
    }

#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}
