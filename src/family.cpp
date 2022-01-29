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

#include "network/family.h"             // Family, operator<<(),
                                        // std::ostream
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // PF_INET, PF_INET6, PF_UNIX, PF_UNSPEC
#endif

auto Network::operator<<(std::ostream& os,
                         const Family& family) noexcept -> std::ostream&
{
    switch (static_cast<family_type>(family)) {
    case AF_UNSPEC:
        os << "AF_UNSPEC";
        break;
    case AF_UNIX:
        os << "AF_UNIX";
        break;
    case AF_INET:
        os << "AF_INET";
        break;
    case AF_INET6:
        os << "AF_INET6";
        break;
    default:
        os << static_cast<family_type>(family);
    }

    return os;
}
