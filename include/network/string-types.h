#ifndef NETWORK_STRING_TYPES_H
#define NETWORK_STRING_TYPES_H

#include "network/optionalstring.h"     // OptionalString

namespace Network
{
    using Hostname = OptionalString;
    using Pathname = OptionalString;
    using Service = OptionalString;
}

#endif
