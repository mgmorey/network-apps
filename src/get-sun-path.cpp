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

#include "network/get-sun-path.h"       // Bytes, Pathname
                                        // get_sun_path()
#include "network/get-sun-length.h"     // get_sun_path_length(), sockaddr_un
#include "network/get-sun-pointer.h"    // get_sun_pointer()

#ifndef _WIN32

auto Network::get_sun_path(const Bytes& addr,
                           Pathname path) -> Pathname
{
    const auto *const sun {get_sun_pointer(addr)};

    if (offsetof(sockaddr_un, sun_path) <= addr.size()) {
        path = sun->sun_path;
    }

    return path;
}

#endif
