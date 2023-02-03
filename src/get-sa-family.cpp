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

#include "network/get-sa-family.h"      // ByteString,
                                        // get_sa_family(),
                                        // socket_family_type
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr
#include "network/sa-offsets.h"         // sa_family_offset

auto Network::get_sa_family(const ByteString& addr,
                            socket_family_type family) noexcept ->
    Network::socket_family_type
{
    if (addr.empty()) {
        return family;
    }

    const auto* const sa {get_sa_pointer(addr)};

    if (addr.size() < sa_family_offset + sizeof sa->sa_family) {
        return family;
    }

    if (sa->sa_family == 0) {
        return family;
    }

    family = sa->sa_family;
    return family;
}
