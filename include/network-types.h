#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include <cstddef>      // std::byte
#include <string>       // std::string

namespace Network
{
    typedef std::basic_string<std::byte> ByteString;

    typedef std::string Hostname;
    typedef std::string Service;
    typedef ByteString SockAddr;
}

#endif
