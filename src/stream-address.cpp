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

#include "network/address.hpp"          // Address,
                                        // operator<<()
#include "network/family-type.hpp"      // family_type
#include "network/format.hpp"           // Format
#include "network/os-features.hpp"      // HAVE_SOCKADDR_SA_LEN
#include "network/socketfamily.hpp"     // SocketFamily

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX
#endif

#include <ostream>      // std::ostream
#include <string>       // std::string
#include <string_view>  // std::string_view

static auto get_prefix(Network::family_type family) noexcept ->
    std::string_view
{
    switch (family) {
    case AF_INET:
        return "sin_";
        break;
    case AF_INET6:
        return "sin6_";
        break;
    case AF_UNIX:
        return "sun_";
        break;
    default:
        return "sa_";
    }
}

static auto get_suffix(Network::family_type family) noexcept ->
    std::string_view
{
    switch (family) {
    case AF_INET:
        return "_in";
        break;
    case AF_INET6:
        return "_in6";
        break;
    case AF_UNIX:
        return "_un";
        break;
    default:
        return {};
    }
}

auto Network::operator<<(std::ostream& os,
                         const Address& address) -> std::ostream&
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    const auto family {address.family()};
    const auto prefix {::get_prefix(family)};
    const auto suffix {::get_suffix(family)};
    os << "sockaddr"
       << suffix
       << '(';

#ifdef HAVE_SOCKADDR_SA_LEN
    os << Format(std::string {prefix} + "len")
       << address.length()
       << Format(delim, tab, std::string {prefix} + "family");
#else
    os << Format(std::string {prefix} + "family");
#endif
    os << SocketFamily(family);

    switch (family) {
    case AF_INET:
    case AF_INET6:
        os << Format(delim, tab, std::string {prefix} + "port")
           << address.port()
           << Format(delim, tab, std::string {prefix} + "addr");
        break;
    case AF_UNIX:
        os << Format(delim, tab, std::string {prefix} + "path");
        break;
    default:
        os << Format(delim, tab, std::string {prefix} + "data");
    }

    os << address.text()
       << ')';
    return os;
}
