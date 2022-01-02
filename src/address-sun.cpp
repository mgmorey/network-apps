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

#include "network/address.h"            // Address, sockaddr_un,
                                        // value_type
#include "network/get-sun-path.h"       // get_sun_path(),
                                        // get_sun_path_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/offsets.h"            // sun_path_offset
#include "network/to-string.h"          // to_string()

#include <string>       // std::string

#ifndef _WIN32

auto Network::Address::sun() const -> const sockaddr_un*
{
    return get_sun_pointer(m_value);
}

auto Network::Address::sun_text() const -> std::string
{
    return get_sun_path(m_value);
}

#endif
