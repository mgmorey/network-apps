#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include "network-optional.h"   // Optional

#include <cstddef>      // std::byte
#include <string>       // std::string

namespace Network
{
    using Byte = std::byte;
    using ByteString = std::basic_string<Byte>;

    using Pathname = std::string;
    using SockAddr = ByteString;

    using Hostname = Optional;
    using Service = Optional;
}

#endif
