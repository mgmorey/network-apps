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
#include "network/get-sin-port.h"       // Bytes, port_type,
                                        // get_sin_port()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/sin-offsets.h"        // sin_port_offset

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, ntohs()
#else
#include <netinet/in.h>     // ntohs()
#include <sys/socket.h>     // AF_INET
#endif

auto Network::get_sin_port(const Bytes& addr,
                           port_type port) -> Network::port_type
{
    assert(get_sa_family(addr) == AF_INET);  // NOLINT
    const auto *const sin {get_sin_pointer(addr)};

    if (addr.size() < sin_port_offset + sizeof sin->sin_port) {
        return port;
    }

    return ntohs(sin->sin_port);
}
