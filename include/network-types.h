#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include "network-bytes.h"    // Byte, ByteString
#include "network-string.h"   // String

namespace Network
{
    using Hostname = String;
    using Pathname = String;
    using Service = String;
    using SockAddr = ByteString;
}

#endif
