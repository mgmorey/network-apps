#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // INVALID_SOCKET, SOCKET
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(-1)
#endif

namespace Network
{
#ifdef _WIN32
    typedef SOCKET fd_type;
#else
    typedef int fd_type;
#endif

    enum { fd_null = INVALID_SOCKET };
}

#endif
