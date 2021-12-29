#ifndef NETWORK_GET_SOCKADDR_H
#define NETWORK_GET_SOCKADDR_H

#include "network/bytespan.h"           // ByteSpan
#include "network/pathname.h"           // Pathname
#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr, sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
    extern auto get_sockaddr() -> SockAddr;
    extern auto get_sockaddr(const ByteSpan& span) -> SockAddr;
    extern auto get_sockaddr(const sockaddr* sa,
                             std::size_t size) -> SockAddr;
#ifndef _WIN32
    extern auto get_sockaddr(const sockaddr_un* sun,
                             std::size_t size = sizeof *sun) -> SockAddr;
    extern auto get_sockaddr(const Pathname& path) -> SockAddr;
#endif
}

#endif
