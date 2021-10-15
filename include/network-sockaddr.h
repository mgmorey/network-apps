#ifndef NETWORK_SOCKADDR_H
#define NETWORK_SOCKADDR_H

#include "network-socklen.h"    // socklen_type
#include "network-types.h"      // SockAddr

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
    extern int get_family(const SockAddr& sock_addr);
    extern socklen_type get_length(const SockAddr& sock_addr);
    extern std::size_t get_max_size(const SockAddr& sock_addr);
    extern const sockaddr* get_pointer(const SockAddr& sock_addr);
    extern sockaddr* get_pointer(SockAddr& sock_addr);
    extern SockAddr get_sockaddr(const sockaddr* sa = nullptr,
                                 std::size_t size = 0);
#ifndef _WIN32
    extern SockAddr get_sockaddr(const sockaddr_un* sun,
                                 std::size_t size = sizeof(sockaddr_un));
    extern SockAddr get_sockaddr(const Pathname& path);
#endif
    extern SockAddr get_sockaddr(const sockaddr_in* sin);
    extern SockAddr get_sockaddr(const sockaddr_in6* sin6);
    extern bool is_valid(const SockAddr& sock_addr, bool verbose = false);
}

#endif
