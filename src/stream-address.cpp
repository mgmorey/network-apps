#include "network-address.h"    // Address, operator<<(),
                                // std::ostream, std::string
#include "network-family.h"     // Family, operator<<()
#include "network-format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX
#endif

static std::string get_prefix(Network::Address::family_type family)
{
    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        return "sun";
        break;
#endif
    case AF_INET:
        return "sin";
        break;
    case AF_INET6:
        return "sin6";
        break;
    default:
        return "sa";
    }
}

static std::string get_suffix(Network::Address::family_type family)
{
    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        return "un";
        break;
#endif
    case AF_INET:
        return "in";
        break;
    case AF_INET6:
        return "in6";
        break;
    default:
        return "";
    }
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Address& address)
{
    static const std::string delim {", "};
    static const int tab {0};

    if (address.empty()) {
        os << "0x0";
    }
    else {
        const Family family {address.family()};
        const auto port {address.port()};
        const auto prefix {get_prefix(family)};
        const auto suffix {get_suffix(family)};
        const auto text {address.text()};

        os << (suffix.empty() ? "sockaddr" : "sockaddr_")
           << suffix
           << '(';

#ifdef HAVE_SOCKADDR_SA_LEN
        const auto length {address.length()};
        os << Format(prefix + "_len")
           << length
           << Format(delim, tab, prefix + "_family")
           << family;
#else
        os << Format(prefix + "_family")
           << family;
#endif

        switch (family) {
#ifndef _WIN32
        case AF_UNIX:
            os << Format(delim, tab, prefix + "_path")
               << text;
            break;
#endif
        case AF_INET:
        case AF_INET6:
            os << Format(delim, tab, prefix + "_port")
               << port
               << Format(delim, tab, prefix + "_addr")
               << text;
            break;
        default:
            os << Format(delim, tab, prefix + "_data")
               << text;
        }

        os << ')';
    }

    return os;
}
