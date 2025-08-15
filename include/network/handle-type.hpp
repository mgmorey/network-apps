// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_HANDLE_TYPE_HPP
#define NETWORK_HANDLE_TYPE_HPP

#ifdef _WIN32
#include <winsock2.h>       // SOCKET
#endif

namespace Network
{
#ifdef _WIN32
    using handle_type = SOCKET;
#else
    using handle_type = int;
#endif
}

#endif
