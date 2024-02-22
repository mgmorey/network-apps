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

#include "network/get-sin-port.h"       // get_sin_port()
#include "network/bytestring.h"         // ByteString
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/port-type.h"          // port_type,
#include "network/sin-offsets.h"        // sin_port_offset

#ifdef WIN32
#include <winsock2.h>       // AF_INET, ntohs()
#else
#include <netinet/in.h>     // ntohs()
#include <sys/socket.h>     // AF_INET
#endif

auto Network::get_sin_port(const ByteString& addr) -> port_type
{
    const auto* const sin {get_sin_pointer(addr)};
    return ntohs(sin->sin_port);
}
