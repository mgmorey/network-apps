#ifndef NETWORK_GET_SOCKADDR_H
#define NETWORK_GET_SOCKADDR_H

#include "network/pathname.h"           // Pathname
#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <optional>     // std::nullopt

namespace Network
{
    extern auto get_sockaddr(const sockaddr* sa = nullptr,
                             std::size_t size = 0) -> SockAddr;
    extern auto get_sockaddr(const sockaddr_in* sin) -> SockAddr;
    extern auto get_sockaddr(const sockaddr_in6* sin6) -> SockAddr;
#ifndef _WIN32
    extern auto get_sockaddr(const Pathname& pathname) -> SockAddr;
#endif
}

#endif
