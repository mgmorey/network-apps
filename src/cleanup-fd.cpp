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

#include "network/cleanup-fd.h"         // cleanup()
#include "network/fd-null.h"            // fd_null
#include "network/get-peername.h"       // get_peername()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sockname.h"       // get_sockname()
#include "network/get-sun-path.h"       // get_sun_path()
#include "network/unlink.h"             // unlink()

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#endif

#include <cerrno>       // ENOENT
#include <iostream>     // std::cerr, std::endl

auto Network::cleanup(fd_type handle, bool verbose) -> void
{
#ifdef WIN32
    static_cast<void>(handle);
    static_cast<void>(verbose);
#else
    if (handle != fd_null) {
        const auto addr {get_sockname(handle, verbose)};

        if (get_sa_family(addr) == AF_UNIX) {
            const auto pathname {get_sun_path(addr)};

            if (pathname) {
                const auto error {unlink(*pathname, verbose)};
                const auto code {error.number()};

                if (code != 0 && code != ENOENT) {
                    std::cerr << error.string()
                              << std::endl;
                }
            }
        }
    }
#endif
}
