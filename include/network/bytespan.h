#ifndef NETWORK_BYTESPAN_H
#define NETWORK_BYTESPAN_H

#include "network/byte.h"           // Byte

#include <span>         // std::span

namespace Network
{
    using ByteSpan = std::span<const Byte>;
}

#endif
