#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#include "network/socklen.h"        // socklen_type
#include "network/types.h"          // SockAddr

#ifdef _WIN32
#include <winsock2.h>       // sockaddr, sockaddr_in
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <ostream>      // std::ostream

namespace Network
{
    extern auto get_family(const SockAddr& sock_addr) -> int;
    extern auto get_length(const SockAddr& sock_addr) -> socklen_type;
    extern auto get_pointer(const SockAddr& sock_addr) -> const sockaddr*;
    extern auto get_pointer(SockAddr& sock_addr) -> sockaddr*;
    extern auto get_sockaddr(const sockaddr* sa = nullptr,
                             std::size_t size = 0) -> SockAddr;
    extern auto get_sockaddr(const sockaddr_in* sin) -> SockAddr;
    extern auto get_sockaddr(const sockaddr_in6* sin6) -> SockAddr;
#ifndef _WIN32
    extern auto get_sockaddr(const sockaddr_un* sun,
                             std::size_t size = sizeof(sockaddr_un)) ->
        SockAddr;
    extern auto get_sockaddr(const Pathname& path) -> SockAddr;
#ifdef HAVE_SOCKADDR_SA_LEN
    extern auto get_sun_length(const sockaddr_un* sun,
                               std::size_t size) -> std::size_t;
#endif
    extern auto get_sun_path_length(const sockaddr_un* sun,
                                    std::size_t size) -> std::size_t;
#endif
    extern auto is_valid(const SockAddr& sock_addr,
                         bool verbose = false) -> bool;
}

#endif
