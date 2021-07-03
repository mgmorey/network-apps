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
            os << static_cast<const sockaddr_un&>(address);
            break;
#endif
        case AF_INET:
            os << static_cast<const sockaddr_in&>(address);
            break;
        case AF_INET6:
            os << static_cast<const sockaddr_in6&>(address);
            break;
        default:
            os << "sockaddr("
               << Format("sa_len")
               << length
               << Format(delim, tabs[0], "sa_family")
               << Family(family)
               << Format(delim, tabs[0], "sa_data")
               << Address::to_string(address.sa_data())
               << ')';
        }
    }
    else {
        os << "0x0";
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const sockaddr_in& sin)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    os << "sockaddr_in("
       << Format("sin_family")
       << Family(sin.sin_family)
       << Format(delim, tabs[0], "sin_port")
       << htons(sin.sin_port)
       << Format(delim, tabs[0], "sin_addr")
       << sin.sin_addr
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const in_addr& addr)
{
    os << "in_addr("
       << Address::to_string(addr)
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const sockaddr_in6& sin6)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    os << "sockaddr_in6("
       << Format("sin6_family")
       << Family(sin6.sin6_family)
       << Format(delim, tabs[0], "sin6_port")
       << htons(sin6.sin6_port)
       << Format(delim, tabs[0], "sin6_addr")
       << sin6.sin6_addr
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const in6_addr& addr)
{
    os << "in6_addr("
       << Address::to_string(addr)
       << ')';
    return os;
}

#ifndef _WIN32
std::ostream& Network::operator<<(std::ostream& os,
                                  const sockaddr_un& sun)
{
    static const std::string delim(", ");
    static const int tabs[1] = {0};

    os << "sockaddr_un("
       << Format("sun_family")
       << Family(sun.sun_family)
       << Format(delim, tabs[0], "sun_path")
       << '"' << sun.sun_path << '"'
       << ')';
    return os;
}
#endif
