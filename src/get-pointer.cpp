#include "network/get-pointer.h"        // SockAddr, get_pointer(),
                                        // sockaddr,

#include <cassert>      // assert()

auto Network::get_pointer(const SockAddr& addr) -> const sockaddr*
{
    const auto *const sa {reinterpret_cast<const sockaddr*>(addr.data())};
    assert(sa != nullptr);  // NOLINT
    return sa;
}

auto Network::get_pointer(SockAddr& addr) -> sockaddr*
{
    auto *const sa {reinterpret_cast<sockaddr*>(addr.data())};
    assert(sa != nullptr);  // NOLINT
    return sa;
}
