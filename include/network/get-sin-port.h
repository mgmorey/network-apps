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

#ifndef NETWORK_GET_SIN_PORT_H
#define NETWORK_GET_SIN_PORT_H

#include "network/bytes.h"              // Bytes
#include "network/port-type.h"          // port_type

namespace Network
{
    extern auto get_sin_port(const Bytes& addr,
                             port_type port = 0) -> port_type;
}

#endif
