#include "network/get-sockaddr.h"       // Byte, SockAddr,
                                        // get_sockaddr(),
                                        // sockaddr_storage,
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan(), std::size_t
#include "network/get-max-size.h"       // get_max_size()

auto Network::get_sockaddr() -> Network::SockAddr
{
    const auto data {static_cast<Byte>(0)};
    const auto size {get_max_size()};
    return {size, data};
}
