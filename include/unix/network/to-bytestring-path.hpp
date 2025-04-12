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

#ifndef UNIX_NETWORK_TO_BYTESTRING_PATH_H
#define UNIX_NETWORK_TO_BYTESTRING_PATH_H

#ifndef WIN32

#include "network/bytestring.hpp"       // ByteString

#include <string_view>      // std::string_view

namespace Network
{
    extern auto to_bytestring(std::string_view path) -> ByteString;
}

#endif

#endif
