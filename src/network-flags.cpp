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
    std::size_t n = 0;

    for(const struct values* p = values; p->flag; ++p) {
        if (flags.value & p->flag) {
            if (n) {
                oss << " | ";
            }

            oss << p->label;
            ++n;
        }
    }

    if (n > 1) {
        os << '(' << oss.str() << ')';
    }
    else if (n > 0) {
        os << oss.str();
    }
    else {
        os << 0;
    }

    return os;
}
