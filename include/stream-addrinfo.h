#ifndef STREAM_ADDRINFO_H
#define STREAM_ADDRINFO_H

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <ostream>      // std::ostream

namespace Network
{
    extern std::ostream& operator<<(std::ostream& os,
                                    const addrinfo& ai);
}

#endif
