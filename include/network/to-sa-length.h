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

#ifndef NETWORK_TO_SA_LENGTH_H
#define NETWORK_TO_SA_LENGTH_H

#include "network/family-type.h"                // family_type
#include "network/length-type.h"                // length_type
#include "network/socketlimits.h"               // SocketLimits
#include "network/to-value.h"                   // to_value()

#ifdef WIN32
#include <winsock2.h>           // AF_UNSPEC
#else
#include <sys/socket.h>         // AF_UNSPEC
#endif

namespace Network
{
    auto to_sa_length(auto value, family_type family = AF_UNSPEC) -> length_type
    {
        const auto [size_min, size_max] {SocketLimits(family).limits()};
        return to_value<length_type>("sa_length_type", value,
                                     size_min,
                                     size_max);
    }
}

#endif
