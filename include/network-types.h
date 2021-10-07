#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include <cstddef>      // std::byte
#include <string>       // std::string

namespace Network
{
    using Byte = std::byte;
    using ByteString = std::basic_string<Byte>;

    using Hostname = std::string;
    using Pathname = std::string;
    using Service = std::string;
    using SockAddr = ByteString;
}

#endif
