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

#include "network/validate-bs.h"                // validate
#include "network/bytestring.h"                 // ByteString
#include "network/familyerror.h"                // FamilyError
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sin-pointer.h"            // get_sin_pointer()
#include "network/get-sin6-pointer.h"           // get_sin6_pointer()
#include "network/get-sun-pointer.h"            // get_sun_pointer()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

auto Network::validate(const ByteString& addr) -> void
{
    switch (const auto family {get_sa_family(addr)}) {
#ifndef WIN32
    case AF_UNIX:
        static_cast<void>(get_sun_pointer(addr));
        break;
#endif
    case AF_INET:
        static_cast<void>(get_sin_pointer(addr));
        break;
    case AF_INET6:
        static_cast<void>(get_sin6_pointer(addr));
        break;
    case AF_UNSPEC:
        break;
    default:
        throw FamilyError(family);
    }
}
