#include "stream-address.h"     // operator<<()
#include "network-address.h"    // Address, operator<<(), struct
                                // sockaddr, socklen_t, std::ostream,
                                // std::string
#include "network-family.h"     // Family, operator<<()
#include "network-format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#endif

std::ostream& Network::operator<<(std::ostream& os,
                                  const Address& address)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    const Address::family_type family = address.sa_family();
    const socklen_t length = address.sa_length();

    if (address.value.size()) {
        switch (family) {
#ifndef _WIN32
        case AF_UNIX:
            os << "sockaddr_un("
               << Format("sun_path")
               << address.sun_text()
               << ')';
            break;
#endif
        case AF_INET:
            os << "sockaddr_sin("
               << Format("sin_addr")
               << address.sin_text()
               << ')';
            break;
        case AF_INET6:
            os << "sockaddr_sin6("
               << Format("sin6_addr")
               << address.sin6_text()
               << ')';
            break;
        default:
            os << "sockaddr("
               << Format("sa_len")
               << length
               << Format(delim, tabs[0], "sa_family")
               << Family(family)
               << Format(delim, tabs[0], "sa_data")
               << address.sa_text()
               << ')';
        }
    }
    else {
        os << "0x0";
    }

    return os;
}
