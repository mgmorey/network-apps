#ifndef NETWORK_BYTESTRING_H
#define NETWORK_BYTESTRING_H

#include "network/byte.h"           // Byte

#include <string>       // std::basic_string

namespace Network
{
    using ByteString = std::basic_string<Byte>;

    extern std::ostream& operator<<(std::ostream& os,
                                    const ByteString& string);
}

#endif
