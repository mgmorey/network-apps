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

#include "network/get-sun-pointer.h"            // get_sun_pointer()
#include "network/bytestring.h"                 // ByteString
#include "network/familyerror.h"                // FamilyError
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/sun-len-limits.h"             // sun_len_min,
                                                // sun_len_max
#include "network/sunlengtherror.h"             // SunLengthError

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#include <sys/un.h>         // sockaddr_un
#endif

#ifndef WIN32

namespace {
    auto validate_sun(const Network::ByteString& addr) -> void
    {
        if (Network::get_sa_family(addr) != AF_UNIX) {
            throw Network::FamilyError();
        }

        if (addr.size() < Network::sun_len_min ||
            addr.size() > Network::sun_len_max) {
            throw Network::SunLengthError(std::to_string(addr.size()),
                                          Network::sun_len_max);
        }
    }
}

auto Network::get_sun_pointer(const ByteString& addr) -> const sockaddr_un*
{
    validate_sun(addr);
    const void* pointer = addr.data();
    return static_cast<const sockaddr_un*>(pointer);
}

// cppcheck-suppress constParameterReference
auto Network::get_sun_pointer(ByteString& addr) -> sockaddr_un*
{
    validate_sun(addr);
    void* pointer = addr.data();
    return static_cast<sockaddr_un*>(pointer);
}

#endif
