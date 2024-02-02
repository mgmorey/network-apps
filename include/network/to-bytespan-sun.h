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

#ifndef NETWORK_TO_BYTESPAN_SUN_H
#define NETWORK_TO_BYTESPAN_SUN_H

#include "network/bytespan.h"                   // ByteSpan
#include "network/sun-len-limits.h"             // sun_len_max
#include "network/sun-len-type.h"               // sun_len_type

#ifndef WIN32
#include <sys/un.h>         // sockaddr_un
#endif

namespace Network
{
#ifndef WIN32
    extern auto to_bytespan(const sockaddr_un* sun,
                            sun_len_type sun_len = sun_len_max) noexcept -> ByteSpan;
#endif
}

#endif
