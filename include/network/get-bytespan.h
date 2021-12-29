#ifndef NETWORK_GET_BYTESPAN_H
#define NETWORK_GET_BYTESPAN_H

#include "network/bytespan.h"           // ByteSpan

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

#include <span>         // std::span

namespace Network
{
    extern auto get_bytespan(const sockaddr* psa,
                             std::size_t size) -> ByteSpan;
}

#endif
