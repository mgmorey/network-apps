#ifndef NETWORK_GET_BYTE_POINTER_H
#define NETWORK_GET_BYTE_POINTER_H

#include "network/byte.h"               // Byte

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

namespace Network
{
    extern auto get_byte_pointer(const sockaddr* sa) -> const Byte*;
    extern auto get_byte_pointer(sockaddr* sa) -> Byte*;
}

#endif
