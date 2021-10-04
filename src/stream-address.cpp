#include "network-address.h"    // Address, operator<<(), sockaddr,
                                // std::ostream, std::string
#include "network-family.h"     // Family, operator<<()
#include "network-format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // ntohs()
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // ntohs()
#endif

std::ostream& Network::operator<<(std::ostream& os,
                                  const Address& address)
{
    static const std::string delim {", "};
    static const int tab {0};

    if (address.empty()) {
        os << "0x0";
    }
    else {
        const auto family {address.family()};
        const auto port {address.port()};
        const auto text {address.text()};

        switch (family) {
#ifndef _WIN32
        case AF_UNIX:
            os << "sockaddr_un("
               << Format("sun_path")
               << text
               << ')';
            break;
#endif
        case AF_INET:
            os << "sockaddr_sin("
               << Format("sin_port")
               << port
               << Format(delim, tab, "sin_addr")
               << text
               << ')';
            break;
        case AF_INET6:
            os << "sockaddr_sin6("
               << Format("sin6_port")
               << port
               << Format(delim, tab, "sin6_addr")
               << text
               << ')';
            break;
        default:
            os << "sockaddr("
               << Format("sa_family")
               << Family(family)
               << Format(delim, tab, "sa_data")
               << text
               << ')';
        }
    }

    return os;
}
