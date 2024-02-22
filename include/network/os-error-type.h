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

#ifndef NETWORK_OS_ERROR_TYPE_H
#define NETWORK_OS_ERROR_TYPE_H

#ifdef WIN32
#include <winsock2.h>       // Always include winsock2.h before
                            // windows.h on Windows
#include <windows.h>        // DWORD
#endif

#include <cstdint>      // uint32_t

namespace Network
{
#ifdef WIN32
    using os_error_type = uint32_t;
#else
    using os_error_type = int;
#endif
}

#endif
