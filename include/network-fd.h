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
    typedef SOCKET sock_fd_type;
#else
    typedef int sock_fd_type;
#endif

    enum { sock_fd_null = INVALID_SOCKET };
}

#endif
