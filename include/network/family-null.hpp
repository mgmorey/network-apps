// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_FAMILY_NULL_HPP
#define NETWORK_FAMILY_NULL_HPP

#include "network/family-type.hpp"      // family_type

#ifdef _WIN32
#include <winsock2.h>           // AF_UNSPEC
#else
#include <sys/socket.h>         // AF_UNSPEC
#endif

namespace Network
{
    constexpr family_type family_null {AF_UNSPEC};
}

#endif
