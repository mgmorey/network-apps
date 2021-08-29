#include "network-flags.h"      // Flags

#ifdef _WIN32
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
                        // AI_NUMERICHOST, AI_NUMERICSERV, AI_PASSIVE,
                        // AI_V4MAPPED
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
                        // AI_NUMERICHOST, AI_NUMERICSERV, AI_PASSIVE,
                        // AI_V4MAPPED
#endif

#include <sstream>      // std::ostringstream
#include <utility>      // std::pair

Network::Flags::Flags(int t_value) :
    m_value(t_value)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Flags& flags)
{
    static const std::pair<int, const char*> values[] = {
        {AI_PASSIVE,                "AI_PASSIVE"},
        {AI_CANONNAME,              "AI_CANONNAME"},
        {AI_NUMERICHOST,            "AI_NUMERICHOST"},
        {AI_NUMERICSERV,            "AI_NUMERICSERV"},
        {AI_ALL,                    "AI_ALL"},
        {AI_ADDRCONFIG,             "AI_ADDRCONFIG"},
        {AI_V4MAPPED,               "AI_V4MAPPED"},
    };

    std::ostringstream oss;
    std::size_t i = 0;

    for(const auto& value : values) {
        if (flags.m_value & value.first) {
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
