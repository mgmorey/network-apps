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

#ifndef WIN32

#include "network/to-path.h"                    // to_path()
#include "network/bytestring.h"                 // ByteString
#include "network/get-path-pointer-sun.h"       // get_path_pointer()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#include "network/optionalpathname.h"           // OptionalPathname
#include "network/sun-length-limits.h"          // sun_length_min

#include <cstring>      // ::strnlen()
#include <optional>     // std::nullopt
#include <string>       // std::string

auto Network::to_path(const ByteString& addr) -> OptionalPathname
{
    const auto* const sun {get_sun_pointer(addr)};
    const auto sun_len {addr.size()};

    if (sun_len == sun_length_min) {
        return std::nullopt;
    }

    const auto* const path {get_path_pointer(sun)};
    const auto path_len {sun_len - sun_length_min};
    return std::string {path, ::strnlen(path, path_len - 1)};
}

#endif
