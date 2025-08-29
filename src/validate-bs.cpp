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

#include "network/validate-bs.hpp"              // validate
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/familyerror.hpp"              // FamilyError
#include "network/get-sa-family.hpp"            // get_sa_family()
#include "network/get-sin-pointer.hpp"          // get_sin_pointer()
#include "network/get-sin6-pointer.hpp"         // get_sin6_pointer()
#ifndef _WIN32
#include "network/get-sun-pointer.hpp"          // get_sun_pointer()
#endif

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX
#endif

auto Network::validate(ByteSpan bs) -> ByteSpan
{
    switch (const auto family {get_sa_family(bs)}) {
    case AF_INET:
        static_cast<void>(get_sin_pointer(bs));
        break;
    case AF_INET6:
        static_cast<void>(get_sin6_pointer(bs));
        break;
#ifndef _WIN32
    case AF_UNIX:
        static_cast<void>(get_sun_pointer(bs));
        break;
#endif
    default:
        throw FamilyError {family};
    }

    return bs;
}
