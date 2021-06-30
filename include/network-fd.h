#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // SOCKET
#endif

namespace Network
{
#ifdef _WIN32
    typedef SOCKET fd_type;
#else
    typedef int fd_type;
#endif
}

#endif
