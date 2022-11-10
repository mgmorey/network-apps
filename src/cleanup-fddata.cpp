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

#include "network/cleanup-fddata.h"             // FdData, cleanup()
#include "network/get-sun-path-fddata.h"        // get_sun_path()
#include "network/unlink.h"                     // unlink()

#include <cerrno>       // ENOENT
#include <iostream>     // std::cerr, std::endl

auto Network::cleanup(FdData& fd_data) -> void
{
    if (const auto pathname {get_sun_path(fd_data)}) {
        if (const auto error {unlink(*pathname, fd_data.verbose())}) {
            if (error.number() != ENOENT) {
                std::cerr << *pathname
                          << ": "
                          << error.string()
                          << std::endl;
            }
        }
    }
}
