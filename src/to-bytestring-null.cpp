// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/to-bytestring-null.h"         // to_bytestring()
#include "network/to-bytestring-path.h"         // to_bytestring()
#include "network/bytestring.h"                 // ByteString

#include <cstddef>      // std::nullptr_t
#include <string_view>  // std::string_view

auto Network::to_bytestring(const std::nullptr_t& path) -> ByteString
{
    static_cast<void>(path);
    return to_bytestring(std::string_view {});
}

#endif
