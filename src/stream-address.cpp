#include "network/address.h"    // Address, operator<<(),
                                // std::ostream, std::string
#include "network/family.h"     // Family, operator<<()
#include "network/format.h"     // Format, operator<<()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX
#endif

static auto get_prefix(Network::family_type family) -> std::string
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

static auto get_suffix(Network::family_type family) -> std::string
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

auto Network::operator<<(std::ostream& os,
                         const Address& address) -> std::ostream&
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    if (address.empty()) {
        os << "0x0";
        return os;
    }

    const Family family {address.family()};
    const auto length {address.length()};
    const auto prefix {::get_prefix(family)};
    const auto suffix {::get_suffix(family)};
    os << (suffix.empty() ? "sockaddr" : "sockaddr_")
       << suffix
       << '(';

    if (length != 0) {
        os << Format(prefix + "_len")
           << address.length()
           << Format(delim, tab, prefix + "_family");
    }
    else {
        os << Format(prefix + "_family");
    }

    os << family;

    switch (family) {
#ifndef _WIN32
    case AF_UNIX:
        os << Format(delim, tab, prefix + "_path");
        break;
#endif
    case AF_INET:
    case AF_INET6:
        os << Format(delim, tab, prefix + "_port")
           << address.port()
           << Format(delim, tab, prefix + "_addr");
        break;
    default:
        os << Format(delim, tab, prefix + "_data");
    }

    os << address.text()
       << ')';
    return os;
}
