// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/address.h"                    // Address
#include "network/format.h"                     // Format
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/socket-family-type.h"         // socket_family_type
#include "network/socketfamily.h"               // SocketFamily,
                                                // operator<<()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace {

    auto get_prefix(Network::socket_family_type family) -> std::string
    {
        switch (family) {
        case AF_UNIX:
            return "sun_";
            break;
        case AF_INET:
            return "sin_";
            break;
        case AF_INET6:
            return "sin6_";
            break;
        default:
            return "sa_";
        }
    }

    auto get_suffix(Network::socket_family_type family) -> std::string
    {
        switch (family) {
        case AF_UNIX:
            return "_un";
            break;
        case AF_INET:
            return "_in";
            break;
        case AF_INET6:
            return "_in6";
            break;
        default:
            return {};
        }
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

    const auto family {address.family()};
    const auto prefix {::get_prefix(family)};
    const auto suffix {::get_suffix(family)};
    os << "sockaddr"
       << suffix
       << '(';

#ifdef HAVE_SOCKADDR_SA_LEN
    os << Format(prefix + "len")
       << address.length()
       << Format(delim, tab, prefix + "family");
#else
    os << Format(prefix + "family");
#endif
    os << SocketFamily(family);

    switch (family) {
#ifndef WIN32
    case AF_UNIX:
        os << Format(delim, tab, prefix + "path");
        break;
#endif
    case AF_INET:
    case AF_INET6:
        os << Format(delim, tab, prefix + "port")
           << address.port()
           << Format(delim, tab, prefix + "addr");
        break;
    default:
        os << Format(delim, tab, prefix + "data");
    }

    os << address.text()
       << ')';
    return os;
}
