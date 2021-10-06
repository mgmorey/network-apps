#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include <cstddef>      // std::byte
#include <string>       // std::string

namespace Network
{
    typedef std::byte Byte;
    typedef std::basic_string<Byte> ByteString;

    typedef std::string Hostname;
    typedef std::string Pathname;
    typedef std::string Service;
    typedef ByteString SockAddr;
}

#endif
