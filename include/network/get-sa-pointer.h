#ifndef NETWORK_GET_SA_POINTER_H
#define NETWORK_GET_SA_POINTER_H

#include "network/bytes.h"              // Bytes
#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

namespace Network
{
    extern auto get_sa_pointer(const SockAddr& addr) -> const sockaddr*;
    extern auto get_sa_pointer(SockAddr& addr) -> sockaddr*;
}

#endif
