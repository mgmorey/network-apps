#include "network/protocol.h"   // Protocol

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#else
#include <netinet/in.h> // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
#endif

#include <ostream>      // std::ostream

Network::Protocol::Protocol(Family t_family, int t_value) :
    Integer(t_value),
    m_family(t_family)
{
}

Network::Protocol& Network::Protocol::operator=(int t_value)
{
    m_value = t_value;
    return *this;
}

Network::Family Network::Protocol::family() const
{
    return m_family;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Protocol& protocol)
{
    const auto family {static_cast<int>(protocol.family())};

    switch (family) {
    case AF_INET:
    case AF_INET6:
    case AF_UNSPEC:
        switch (protocol.m_value) {
        case IPPROTO_IP:
            os << "IPPROTO_IP";
            break;
        case IPPROTO_ICMP:
            os << "IPPROTO_ICMP";
            break;
        case IPPROTO_IGMP:
            os << "IPPROTO_IGMP";
            break;
        case IPPROTO_TCP:
            os << "IPPROTO_TCP";
            break;
        case IPPROTO_UDP:
            os << "IPPROTO_UDP";
            break;
        case IPPROTO_RAW:
            os << "IPPROTO_RAW";
            break;
        default:
            os << protocol.m_value;
        }
        break;
    default:
        os << protocol.m_value;
    }

    return os;
}
