#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // SOCK_DGRAM, SOCK_RAW, SOCK_RDM,
                        // SOCK_SEQPACKET, SOCK_STREAM
#else
#include <sys/socket.h> // SOCK_DGRAM, SOCK_DCCP, SOCK_PACKET,
                        // SOCK_RAW, SOCK_RDM, SOCK_SEQPACKET,
                        // SOCK_STREAM
#endif

#include <sstream>      // std::ostringstream

Network::SockType::SockType(int t_value) :
    m_value(t_value)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockType& socktype)
{
    static const struct values {
        int flag;
        const char* label;
    } values[] = {
#ifdef SOCK_CLOEXEC
        {SOCK_CLOEXEC,              "SOCK_CLOEXEC"},
#endif
#ifdef SOCK_NONBLOCK
        {SOCK_NONBLOCK,             "SOCK_NONBLOCK"},
#endif
        {0,                         NULL}
    };
    std::ostringstream oss;
    std::size_t i = 0;

    if (socktype.m_value & 00000017) {
        switch (socktype.m_value & 00000017) {
        case SOCK_STREAM:
            oss << "SOCK_STREAM";
            break;
        case SOCK_DGRAM:
            oss << "SOCK_DGRAM";
            break;
        case SOCK_RAW:
            oss << "SOCK_RAW";
            break;
        case SOCK_RDM:
            oss << "SOCK_RDM";
            break;
        case SOCK_SEQPACKET:
            oss << "SOCK_SEQPACKET";
            break;
        default:
            oss << socktype.m_value;
        }
        ++i;
    }

    for(const struct values* p = values; p->flag; ++p) {
        if (socktype.m_value & p->flag) {
            if (i++ > 0) {
                oss << " | ";
            }

            oss << p->label;
        }
    }

    switch (i) {
    case 0:
        os << 0;
        break;
    case 1:
        os << oss.str();
        break;
    default:
        os << '(' << oss.str() << ')';
    }

    return os;
}
