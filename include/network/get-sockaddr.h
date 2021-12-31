#ifndef NETWORK_GET_SOCKADDR_H
#define NETWORK_GET_SOCKADDR_H

#include "network/bytes.h"              // Bytes
#include "network/bytespan.h"           // ByteSpan
#include "network/pathname.h"           // Pathname
#include "network/sizes.h"              // sun_size

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // std::size_t

namespace Network
{
    extern auto get_sockaddr() -> Bytes;
    extern auto get_sockaddr(const ByteSpan& span) -> Bytes;
    extern auto get_sockaddr(const sockaddr* sa,
                             std::size_t size) -> Bytes;
#ifndef _WIN32
    extern auto get_sockaddr(const sockaddr_un* sun,
                             std::size_t size = sun_size) -> Bytes;
    extern auto get_sockaddr(const Pathname& path) -> Bytes;
#endif
}

#endif
