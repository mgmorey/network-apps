#ifndef NETWORK_GET_MAX_SIZE_H
#define NETWORK_GET_MAX_SIZE_H

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_storage
#else
#include <sys/socket.h>     // sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()

constexpr auto get_max_size() -> std::size_t
{
    constexpr auto storage_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto unix_size {static_cast<std::size_t>(0)};
#else
    constexpr auto unix_size {sizeof(sockaddr_un)};
#endif
    return std::max(storage_size, unix_size);
}

#endif
