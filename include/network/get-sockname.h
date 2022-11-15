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

#ifndef NETWORK_GET_SOCKNAME_H
#define NETWORK_GET_SOCKNAME_H

#include "network/descriptor-type.h"            // descriptor_type
#include "network/sockname.h"                   // SockName

namespace Network
{
    extern auto get_sockname(descriptor_type handle,
                             bool verbose = false) -> SockName;
}

#endif
