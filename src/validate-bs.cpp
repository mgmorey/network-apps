// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/validate-bs.h"        // validate
#include "network/familyerror.h"        // FamilyError
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#ifndef WIN32
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#endif

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span

auto Network::validate(std::span<const std::byte> bs) ->
    std::span<const std::byte>
{
    switch (const auto family {get_sa_family(bs)}) {
#ifndef WIN32
    case AF_UNIX:
        static_cast<void>(get_sun_pointer(bs));
        break;
#endif
    case AF_INET:
        static_cast<void>(get_sin_pointer(bs));
        break;
    case AF_INET6:
        static_cast<void>(get_sin6_pointer(bs));
        break;
    default:
        throw FamilyError(family);
    }

    return bs;
}
