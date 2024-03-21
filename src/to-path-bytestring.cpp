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

#include "network/to-path-bytestring.h"         // to_path()
#include "network/bytestring.h"                 // ByteString
#include "network/get-path-pointer-bs.h"        // get_path_pointer()
#include "network/optionalpathname.h"           // OptionalPathname
#include "network/sun-len-limits.h"             // sun_len_min

#include <cstddef>      // std::size_t
#include <cstring>      // ::strnlen()
#include <string>       // std::string

auto Network::to_path(const ByteString& addr) -> OptionalPathname
{
    const auto* const data {get_path_pointer(addr)};
    const auto size_max {addr.size() - sun_len_min};
    std::size_t size = 0UL;

    if (size_max != 0UL) {
        size = ::strnlen(data, size_max);
    }

    return std::string {data, size};
}

#endif
