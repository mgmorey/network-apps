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

#ifndef NETWORK_HANDLE_NULL_H
#define NETWORK_HANDLE_NULL_H

#include "network/handle-type.h"                // handle_type

#ifdef WIN32
#include <winsock2.h>       // INVALID_SOCKET
#else
#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(-1)	// NOLINT
#endif
#endif

namespace Network
{
    constexpr handle_type handle_null {INVALID_SOCKET};
}

#endif
