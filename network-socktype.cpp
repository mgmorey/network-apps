#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // SOCK_DGRAM, SOCK_STREAM
#else
#include <sys/socket.h> // SOCK_DGRAM, SOCK_STREAM
#endif

Network::SockType::SockType(int socktype) :
    value(socktype)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockType& socktype)
{
    switch(socktype.value) {
    case SOCK_DGRAM:
        os << "SOCK_DGRAM";
        break;
    case SOCK_STREAM:
        os << "SOCK_STREAM";
        break;
    default:
        os << socktype.value;
    }

    return os;
}
