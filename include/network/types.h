#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include "network/byte.h"           // Byte
#include "network/optionalstring.h" // OptionalString

namespace Network
{
    using Hostname = OptionalString;
    using Pathname = OptionalString;
    using Service = OptionalString;
    using SockAddr = ByteString;
}

#endif
