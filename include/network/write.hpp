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

#ifndef NETWORK_WRITE_HPP
#define NETWORK_WRITE_HPP

#include "network/socketcore.hpp"       // SocketCore

#include <sys/types.h>      // ssize_t

#include <string_view>  // std::string_view

namespace Network
{
    extern auto write(const SocketCore& sc, std::string_view sv) -> ssize_t;
}

#endif
