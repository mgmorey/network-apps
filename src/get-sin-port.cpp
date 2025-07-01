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

#include "network/bytespan.hpp"         // ByteSpan
#include "network/get-sin-port.hpp"     // get_sin_port()
#include "network/get-sin-pointer.hpp"  // get_sin_pointer()
#include "network/port-type.hpp"        // port_type

#ifdef WIN32
#include <winsock2.h>       // ntohs()
#else
#include <netinet/in.h>     // ntohs()
#endif

auto Network::get_sin_port(ByteSpan bs) -> port_type
{
    const auto* const sin {get_sin_pointer(bs)};
    return ntohs(sin->sin_port);
}
