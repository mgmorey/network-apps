#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // SOCKET
#endif

namespace Network
{
#ifdef _WIN32
    enum { fd_null = static_cast<SOCKET>(INVALID_SOCKET) };
    typedef SOCKET fd_type;
#else
    enum { fd_null = -1 };
    typedef int fd_type;
#endif
}

#endif
