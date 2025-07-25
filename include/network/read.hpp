// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_READ_HPP
#define NETWORK_READ_HPP

#include "network/charspan.hpp"         // CharSpan
#include "network/socketcore.hpp"       // SocketCore

#include <sys/types.h>      // ssize_t

namespace Network
{
    extern auto read(const SocketCore& sc, CharSpan cs) -> ssize_t;
}

#endif
