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

#ifndef NETWORK_SUN_OFFSETS_H
#define NETWORK_SUN_OFFSETS_H

#ifndef _WIN32
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // offsetof()

namespace Network
{
#ifndef _WIN32
    constexpr auto sun_family_offset {offsetof(sockaddr_un, sun_family)};
    constexpr auto sun_path_offset {offsetof(sockaddr_un, sun_path)};
#endif
}

#endif
