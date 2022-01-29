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

#include "network/assert.h"             // assert()
#include "network/get-sin-addr.h"       // ByteString, get_sin_addr(),
                                        // in_addr
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/os-features.h"        // WIN32
#include "network/sin-offsets.h"        // sin_addr_offset

#ifdef WIN32
#include <winsock2.h>       // AF_INET
#else
#include <sys/socket.h>     // AF_INET
#endif

auto Network::get_sin_addr(const ByteString& addr,
                           const in_addr& ip) noexcept -> in_addr
{
    assert(get_sa_family(addr) == AF_INET);
    const auto *const sin {get_sin_pointer(addr)};

    if (addr.size() < sin_addr_offset + sizeof sin->sin_addr) {
        return ip;
    }

    return sin->sin_addr;
}
