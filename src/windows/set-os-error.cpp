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

#ifdef _WIN32

#include "network/set-os-error.hpp"     // set_os_error()
#include "network/os-error-type.hpp"    // os_error_type

#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // ::SetLastError()

auto Network::set_os_error(os_error_type os_error_code) -> os_error_type
{
    ::SetLastError(os_error_code);
    return os_error_code;
}

#endif
