#ifndef NETWORK_BYTES_H
#define NETWORK_BYTES_H

#include <cstddef>      // std::byte
#include <string>       // std::string

namespace Network
{
    using Byte = std::byte;
    using ByteString = std::basic_string<Byte>;
}

#endif
