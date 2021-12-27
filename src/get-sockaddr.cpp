#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr(),
                                        // sockaddr
#include "network/get-byte-pointer.h"   // get_byte_pointer()

#ifdef _WIN32
#include <winsock2.h>       // sockaddr_storage
#else
#include <sys/socket.h>     // sockaddr_storage
#include <sys/un.h>         // sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <span>         // std::span()

static constexpr auto get_capacity() -> Network::SockAddr::size_type
{
    constexpr auto storage_size {sizeof(sockaddr_storage)};
#ifdef _WIN32
    constexpr auto unix_size {static_cast<std::size_t>(0)};
#else
    constexpr auto unix_size {sizeof(sockaddr_un)};
#endif
    return std::max(storage_size, unix_size);
}

auto Network::get_sockaddr(const sockaddr* sa,
                           std::size_t size) -> Network::SockAddr
{
    assert(size ? sa != nullptr : sa == nullptr);  // NOLINT

    if (sa == nullptr) {
        return {get_capacity(), static_cast<Byte>(0)};
    }

    const auto *const bytes {get_byte_pointer(sa)};
    const auto span {std::span(bytes, size)};
    return {span.data(), span.size()};
}
