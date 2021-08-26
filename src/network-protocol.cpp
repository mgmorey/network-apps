#include "network-protocol.h"   // Protocol

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

Network::Protocol::Protocol(int t_family, int t_value) :
    m_family(t_family),
    m_value(t_value)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Protocol& protocol)
{
    switch(protocol.m_family) {
    case AF_INET:
    case AF_INET6:
        switch(protocol.m_value) {
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
