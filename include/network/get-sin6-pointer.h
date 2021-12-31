#ifndef NETWORK_GET_SIN6_POINTER_H
#define NETWORK_GET_SIN6_POINTER_H

#include "network/bytes.h"              // Bytes

#ifdef _WIN32
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#endif

namespace Network
{
    extern auto get_sin6_pointer(const Bytes& addr) -> const sockaddr_in6*;
    extern auto get_sin6_pointer(Bytes& addr) -> sockaddr_in6*;
}

#endif
