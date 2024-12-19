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

#ifdef WIN32

#include "network/get-last-os-error.h"          // get_last_os_error()
#include "network/os-error-type.h"              // os_error_type

#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // ::GetLastError()

auto Network::get_last_os_error() -> os_error_type
{
    return ::GetLastError();
}

#endif
