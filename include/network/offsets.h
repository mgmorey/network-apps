#ifndef NETWORK_OFFSETS_H
#define NETWORK_OFFSETS_H

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // offsetof(), std::size_t

namespace Network
{
    constexpr auto sa_data_offset {offsetof(sockaddr, sa_data)};
#ifdef _WIN32
    constexpr auto sun_path_offset {static_cast<std::size_t>(0)};
#else
    constexpr auto sun_path_offset {offsetof(sockaddr_un, sun_path)};
#endif
}

#endif
