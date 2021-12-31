#ifndef NETWORK_GET_SIN_POINTER_H
#define NETWORK_GET_SIN_POINTER_H

#include "network/bytes.h"              // Bytes

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_in
#else
#include <netinet/in.h>     // sockaddr_in
#endif

namespace Network
{
    extern auto get_sin_pointer(const Bytes& addr) -> const sockaddr_in*;
    extern auto get_sin_pointer(Bytes& addr) -> sockaddr_in*;
}

#endif
