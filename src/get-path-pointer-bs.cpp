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

#include "network/get-path-pointer.h"   // ByteString,
                                        // get_path_pointer()
#include "network/get-sun-pointer.h"    // get_sun_pointer()

#ifndef WIN32

auto Network::get_path_pointer(const ByteString& addr) noexcept -> const char*
{
    const auto *sun {get_sun_pointer(addr)};
    return get_path_pointer(sun);
}

auto Network::get_path_pointer(ByteString& addr) noexcept -> char*
{
    auto *sun {get_sun_pointer(addr)};
    return get_path_pointer(sun);
}

#endif
