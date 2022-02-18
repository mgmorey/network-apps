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

#include "network/address.h"            // Address, operator<<(),
                                        // std::ostream, std::string
#include "network/format.h"             // Format, operator<<()
#include "network/socketfamily.h"       // SocketFamily, operator<<()

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX
#endif

static auto get_prefix(Network::socket_family_type family) noexcept ->
    std::string
{
    switch (family) {
#ifndef WIN32
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

static auto get_suffix(Network::socket_family_type family) noexcept ->
    std::string
{
    switch (family) {
#ifndef WIN32
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
                         const Address& address) noexcept -> std::ostream&
{
    constexpr auto delim {", "};
    constexpr auto tab {0};

    if (address.empty()) {
        os << "0x0";
        return os;
    }

    const SocketFamily family {address.family()};
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
#ifndef WIN32
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
