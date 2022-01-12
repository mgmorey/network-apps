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

#include "network/to-byte-string-sa.h"  // ByteString, sockaddr,
                                        // std::size_t,
                                        // to_byte_string()
#include "network/to-byte-span.h"       // to_byte_span()
#include "network/to-byte-string.h"     // to_byte_string()

auto Network::to_byte_string(const sockaddr* sa,
                             std::size_t size) -> Network::ByteString
{
    if (sa == nullptr) {
        return {};
    }

    return to_byte_string(to_byte_span(sa, size));
}
