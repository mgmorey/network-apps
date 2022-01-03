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

#include "network/get-sin6-addr.h"      // Bytes, in6_addr,
                                        // get_sin6_addr()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/sin6-offsets.h"       // sin6_addr_offset

auto Network::get_sin6_addr(const Bytes& addr,
                            const in6_addr& ip) -> in6_addr
{
    if (addr.empty()) {
        return ip;
    }

    const auto *const sin6 {get_sin6_pointer(addr)};

    if (addr.size() < sin6_addr_offset + sizeof sin6->sin6_addr) {
        return ip;
    }

    return sin6->sin6_addr;
}
