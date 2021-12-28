#include "network/get-byte-pointer.h"   // Byte, get_byte_pointer(),
                                        // sockaddr

auto Network::get_byte_pointer(const sockaddr* sa) -> const Byte*
{
    return reinterpret_cast<const Byte*>(sa);  // NOLINT
}

auto Network::get_byte_pointer(sockaddr* sa) -> Byte*
{
    return reinterpret_cast<Byte*>(sa);  // NOLINT
}
