#include "network-protocol.h"   // Protocol

#ifdef _WIN32
#include <winsock2.h>   // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
#else
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
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
    case IPPROTO_TCP:
        os << "IPPROTO_TCP";
        break;
    case IPPROTO_UDP:
        os << "IPPROTO_UDP";
        break;
    default:
        os << protocol.value;
    }

    return os;
}
