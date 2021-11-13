#ifndef NETWORK_BYTE_H
#define NETWORK_BYTE_H

#include <cstddef>      // std::byte
#include <string>       // std::basic_string

namespace Network
{
    using Byte = std::byte;
    using ByteString = std::basic_string<Byte>;

    extern std::ostream& operator<<(std::ostream& os,
                                    const ByteString& string);
}

#endif
