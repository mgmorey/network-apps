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

#include "network/get-sin6-port.h"      // get_sin6_port()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/port-type.h"          // port_type

#ifdef WIN32
#include <winsock2.h>       // ntohs()
#else
#include <netinet/in.h>     // ntohs()
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span

auto Network::get_sin6_port(const std::span<const std::byte>& bs) -> port_type
{
    const auto* const sin6 {get_sin6_pointer(bs)};
    return ntohs(sin6->sin6_port);
}
