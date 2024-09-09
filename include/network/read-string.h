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

#ifndef NETWORK_READ_STRING_H
#define NETWORK_READ_STRING_H

#include "network/readresult.h"                 // ReadResult
#include "network/sharedsocket.h"               // SharedSocket

#include <cstddef>      // std::size_t

namespace Network
{
    extern auto read_string(const SharedSocket& sock,
                            std::size_t size) -> ReadResult;
}

#endif
