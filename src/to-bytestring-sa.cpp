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

#include "network/to-bytestring-sa.h"   // ByteString, sockaddr,
                                        // std::size_t,
                                        // to_bytestring()
#include "network/to-bytespan-void.h"   // to_bytespan()
#include "network/to-bytestring.h"      // to_bytestring()

auto Network::to_bytestring(const sockaddr* sa,
                             std::size_t size) -> Network::ByteString
{
    if (sa == nullptr) {
        return {};
    }

    return to_bytestring(to_bytespan(sa, size));
}
