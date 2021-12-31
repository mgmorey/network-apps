#include "network/get-sockaddr.h"       // Byte, Bytes,
                                        // get_sockaddr(),
                                        // sockaddr_storage,
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan(), std::size_t
#include "network/sizes.h"              // sockaddr_size_max

auto Network::get_sockaddr() -> Network::Bytes
{
    const auto data {static_cast<Byte>(0)};
    const auto size {sockaddr_size_max};
    return {size, data};
}
