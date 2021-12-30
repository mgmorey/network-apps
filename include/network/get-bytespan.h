#ifndef NETWORK_GET_BYTESPAN_H
#define NETWORK_GET_BYTESPAN_H

#include "network/bytespan.h"           // ByteSpan
#include "network/sizes.h"              // sun_size

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // std::size_t
#include <span>         // std::span

namespace Network
{
    extern auto get_bytespan(const sockaddr* sa,
                             std::size_t size) -> ByteSpan;
#ifndef _WIN32
    extern auto get_bytespan(const sockaddr_un* sun,
                             std::size_t size = sun_size) -> ByteSpan;
#endif
}

#endif
