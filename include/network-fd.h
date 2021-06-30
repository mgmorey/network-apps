#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // INVALID_SOCKET, SOCKET
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(-1)
#endif

#include <utility>      // std::pair

namespace Network
{
#ifdef _WIN32
    typedef SOCKET fd_type;
#else
    typedef int fd_type;
#endif
    typedef std::pair<fd_type, fd_type> FdPair;

    enum { fd_null = INVALID_SOCKET };
}

#endif
