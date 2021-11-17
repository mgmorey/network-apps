#include "network/socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // SOCK_DGRAM, SOCK_RAW, SOCK_RDM,
                        // SOCK_SEQPACKET, SOCK_STREAM
#else
#include <sys/socket.h> // SOCK_DGRAM, SOCK_DCCP, SOCK_PACKET,
                        // SOCK_RAW, SOCK_RDM, SOCK_SEQPACKET,
                        // SOCK_STREAM
#endif

#include <sstream>      // std::ostringstream
#include <vector>       // std::vector

Network::SockType::SockType(socktype_type t_value) :
    Integer(t_value)
{
}

Network::SockType& Network::SockType::operator=(socktype_type t_value)
{
    m_value = t_value;
    return *this;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockType& socktype)
{
    static const std::vector<std::pair<int, const char*>> values {
#ifdef SOCK_CLOEXEC
        {SOCK_CLOEXEC,              "SOCK_CLOEXEC"},
#endif
#ifdef SOCK_NONBLOCK
        {SOCK_NONBLOCK,             "SOCK_NONBLOCK"},
#endif
        {0,                         nullptr}
    };

    constexpr auto mask {0000017};
    std::ostringstream oss;
    std::size_t i {0};

    if (socktype.m_value & mask) {
        switch (socktype.m_value & mask) {
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

    for(const auto& value : values) {
        if (socktype.m_value & value.first) {
            if (i++ > 0) {
                oss << " | ";
            }

            oss << value.second;
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
