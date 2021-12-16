#include "network/get-sun-length.h"     // SockAddr, get_sun_length(),
                                        // get_sun_path_length(),
                                        // sockaddr_un

#ifndef _WIN32
#include <sys/socket.h>     // sockaddr
#endif

#include <algorithm>    // std::max(), std::min()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <cstring>      // strnlen()

#ifndef _WIN32

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::get_sun_length(const sockaddr_un* sun,
                             std::size_t size) -> std::size_t
{
    const auto path_len {get_sun_path_length(sun, size)};
    const auto min_size {sizeof *sun - sizeof sun->sun_path + path_len};
    return std::max(sizeof(sockaddr), min_size);
}

#endif

auto Network::get_sun_path_length(const sockaddr_un* sun,
                                  std::size_t size) -> std::size_t
{
    static constexpr auto path_offset {offsetof(sockaddr_un, sun_path)};

    assert(size >= path_offset);
    assert(size <= path_offset + sizeof sun->sun_path);
    size = std::max(size, path_offset);
    size = std::min(size, path_offset + sizeof sun->sun_path);
#ifdef HAVE_SOCKADDR_SA_LEN
    std::size_t len {sun->sun_len};
    assert(len >= path_offset);
    assert(len <= size);
    len = std::max(len, path_offset);
    len = std::min(len, size);
    const auto max_path_len {std::min(size, len) - path_offset};
#else
    const auto max_path_len {std::min(size - path_offset,
                                      sizeof sun->sun_path)};
#endif
    return strnlen(sun->sun_path, max_path_len);
}

#endif
