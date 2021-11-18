#include "network/family.h"     // Family

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#endif

Network::Family::Family(family_type t_value) :
    Integer(t_value)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Family& family)
{
    switch (family) {
    case AF_UNSPEC:
        os << "AF_UNSPEC";
        break;
    case AF_UNIX:
        os << "AF_UNIX";
        break;
    case AF_INET:
        os << "AF_INET";
        break;
    case AF_INET6:
        os << "AF_INET6";
        break;
    default:
        os << static_cast<family_type>(family);
    }

    return os;
}
