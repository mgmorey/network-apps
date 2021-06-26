#include "network-family.h"     // Family

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6
#else
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6
#endif

Network::Family::Family(int family) :
    value(family)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Family& family)
{
    switch(family.value) {
    case AF_INET:
        os << "AF_INET";
        break;
    case AF_INET6:
        os << "AF_INET6";
        break;
    case AF_UNSPEC:
        os << "AF_UNSPEC";
        break;
    default:
        os << family.value;
    }

    return os;
}
