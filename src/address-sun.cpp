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
#include "network/get-sun-length.h"     // get_sun_path_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/offsets.h"            // sun_path_offset
#include "network/to-string.h"          // to_string()

#include <string>       // std::string

#ifndef _WIN32

auto Network::Address::sun() const -> const sockaddr_un*
{
    return get_sun_pointer(m_value);
}

auto Network::Address::sun_path() const -> Network::Address::value_type
{
    const auto size {m_value.size()};
    const auto length {
        size < sun_path_offset ? 0 :
        get_sun_path_length(sun(), size)
    };
    const auto offset {sun_path_offset};
    return m_value.substr(offset, length);
}

auto Network::Address::sun_text() const -> std::string
{
    const auto text {to_string(sun_path())};
    return text.substr(0, text.find('\0'));
}

#endif
