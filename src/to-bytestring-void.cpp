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

#include "network/to-bytestring-void.h"         // to_bytestring()
#include "network/byte.h"                       // Byte
#include "network/bytestring.h"                 // ByteString
#include "network/length-type.h"                // length_type

#include <span>         // std::span

auto Network::to_bytestring(const void* data, length_type size) -> ByteString
{
    const std::span<const Byte> span {static_cast<const Byte*>(data), size};
    return {span.begin(), span.end()};
}
