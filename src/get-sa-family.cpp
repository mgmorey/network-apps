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

#include "network/get-sa-family.h"      // Bytes, family_type,
                                        // get_sa_family()
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr
#include "network/sa-offsets.h"         // sa_family_offset

auto Network::get_sa_family(const Bytes& addr,
                            family_type family) -> Network::family_type
{
    const auto *const sa {get_sa_pointer(addr)};

    if (sa_family_offset + sizeof sa->sa_family <= addr.size()) {
        if (sa->sa_family != 0) {
            family = sa->sa_family;
        }
    }

    return family;
}
