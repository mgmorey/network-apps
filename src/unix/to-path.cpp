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

#ifndef _WIN32

#include "network/to-path.hpp"                  // to_path()
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/get-path-pointer.hpp"         // get_path_pointer()
#include "network/get-sun-pointer.hpp"          // get_sun_pointer()
#include "network/sun-offsets.hpp"              // sun_path_offset

#include <string_view>  // std::string_view

auto Network::to_path(ByteSpan bs) -> std::string_view
{
    const auto* const path_str {get_path_pointer(get_sun_pointer(bs))};
    const auto path_len {bs.size() - sun_path_offset};
    const std::string_view path {path_str, path_len};
    return path.substr(0, path.find('\0'));
}

#endif
