#ifndef NETWORK_GET_SA_POINTER_H
#define NETWORK_GET_SA_POINTER_H

#include "network/sockaddr.h"           // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
    extern auto get_sa_pointer(const SockAddr& addr) -> const sockaddr*;
    extern auto get_sa_pointer(SockAddr& addr) -> sockaddr*;
    extern auto get_sa_pointer(const sockaddr_in* sin) -> const sockaddr*;
    extern auto get_sa_pointer(sockaddr_in* sin) -> sockaddr*;
    extern auto get_sa_pointer(const sockaddr_in6* sin6) -> const sockaddr*;
    extern auto get_sa_pointer(sockaddr_in6* sin6) -> sockaddr*;
}

#endif
