#include "network/family.h"     // Family

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#endif

Network::Family::Family(int t_family) :
    Integer(t_family)
{
}

Network::Family& Network::Family::operator=(int t_value)
{
    m_value = t_value;
    return *this;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Family& family)
{
    switch (family.m_value) {
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
        os << family.m_value;
    }

    return os;
}
