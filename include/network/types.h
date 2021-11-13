#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include "network/bytestring.h"     // ByteString
#include "network/optionalstring.h" // OptionalString

namespace Network
{
    using Hostname = OptionalString;
    using Pathname = OptionalString;
    using Service = OptionalString;
    using SockAddr = ByteString;
}

#endif
