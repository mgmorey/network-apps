#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr_un
#include "network/get-sun-length.h"     // get_sun_length()

#ifndef _WIN32
#include <sys/socket.h>     // AF_LOCAL
#endif

#include <algorithm>    // std::max(), std::min()
#include <array>        // std::array
#include <cassert>      // assert()
#include <span>         // std::as_bytes(), std::span

#ifndef _WIN32

auto Network::get_sockaddr(const Pathname& path) -> Network::SockAddr
{
    std::array<sockaddr_un, 1> sun {};
    const auto path_size {std::min(path.size(), sizeof sun[0].sun_path - 1)};
    const auto min_size {sizeof sun[0] - sizeof sun[0].sun_path + path_size};
    const auto size {std::max(sizeof(sockaddr), min_size + 1)};
    path.copy(sun[0].sun_path, path_size);  // NOLINT
    sun[0].sun_family = AF_LOCAL;
#ifdef HAVE_SOCKADDR_SA_LEN
    sun[0].sun_len = std::max(sizeof(sockaddr), min_size);
    assert(sun[0].sun_len == get_sun_length(&sun[0], sizeof sun[0]));  // NOLINT
#endif
    const auto bytes {std::as_bytes(std::span(sun))};
    SockAddr addr {bytes.data(), bytes.size()};
    addr.resize(size);
    return addr;
}

#endif
