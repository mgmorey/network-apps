#include "network-protocol.h"   // Protocol

#ifdef _WIN32
#include <winsock2.h>   // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#else
#include <netinet/in.h> // IPPROTO_ICMP, IPPROTO_IGMP, IPPROTO_IP,
                        // IPPROTO_RAW, IPPROTO_TCP, IPPROTO_UDP
#endif

#include <ostream>      // std::ostream

Network::Protocol::Protocol(int protocol) :
    value(protocol)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Protocol& protocol)
{
    switch(protocol.value) {
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
        os << protocol.value;
    }

    return os;
}
