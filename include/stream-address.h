#ifndef STREAM_ADDRESS_H
#define STREAM_ADDRESS_H

#include "network-address.h"    // Address

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, struct sockaddr_in,
                        // struct sockaddr_in6
#else
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6
#include <sys/socket.h> // struct sockaddr
#include <sys/un.h>     // struct sockaddr_un
#endif

#include <ostream>      // std::ostream

namespace Network
{
    extern std::ostream& operator<<(std::ostream& os,
                                    const Address& address);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in& sin);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in_addr& addr);
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_in6& sin6);
    extern std::ostream& operator<<(std::ostream& os,
                                    const in6_addr& addr);
#ifndef _WIN32
    extern std::ostream& operator<<(std::ostream& os,
                                    const sockaddr_un& sun);
#endif
}

#endif
