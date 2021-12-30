#ifndef NETWORK_SIZES_H
#define NETWORK_SIZES_H

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cstddef>      // std::size_t

namespace Network
{
    constexpr auto ss_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto sun_size {static_cast<std::size_t>(0)};
#else
    constexpr auto sun_size {sizeof(sockaddr_un)};
#endif
    constexpr auto max_size {std::max(ss_size, sun_size)};
}

#endif
