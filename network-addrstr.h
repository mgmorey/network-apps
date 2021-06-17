#ifndef NETWORK_ADDRSTR_H
#define NETWORK_ADDRSTR_H

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <string>       // std::string

namespace Network
{
    extern std::string get_addrstr(const struct addrinfo& other);
}

#endif
