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

#include "network/unlink-fd.h"          // OsErrorResult, fd_type,
                                        // unlink()
#include "network/get-sun-path-fd.h"    // get_sun_path()
#include "network/unlink-path.h"        // unlink()

auto Network::unlink(fd_type handle,
                     bool verbose) -> Network::OsErrorResult
{
    if (const auto pathname {get_sun_path(handle)}) {
        if (auto os_error {unlink(*pathname, verbose)}) {
            return os_error;
        }
    }

    return {};
}
