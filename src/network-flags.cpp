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

Network::Flags::Flags(int flags) :
    value(flags)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Flags& flags)
{
    static struct values {
        int flag;
        const char* label;
    } values[] = {
        {AI_PASSIVE,                "AI_PASSIVE"},
        {AI_CANONNAME,              "AI_CANONNAME"},
        {AI_NUMERICHOST,            "AI_NUMERICHOST"},
        {AI_NUMERICSERV,            "AI_NUMERICSERV"},
        {AI_ALL,                    "AI_ALL"},
        {AI_ADDRCONFIG,             "AI_ADDRCONFIG"},
        {AI_V4MAPPED,               "AI_V4MAPPED"},
        {0,                         NULL}
    };
    std::ostringstream oss;
    std::size_t i = 0;

    for(const struct values* p = values; p->flag; ++p) {
        if (flags.value & p->flag) {
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
