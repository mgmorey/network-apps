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

#include "network/cleanup-fd.h"                 // FdData, cleanup()
#include "network/fd-null.h"                    // fd_null
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sockname.h"               // get_sockname()
#include "network/get-sun-path-bytestring.h"    // get_sun_path()
#include "network/unlink.h"                     // unlink()

#ifndef WIN32
#include <sys/socket.h>     // AF_UNIX
#endif

#include <cerrno>       // ENOENT
#include <iostream>     // std::cerr, std::endl

auto Network::cleanup(FdData& fd_data) -> void
{
#ifndef WIN32
    const auto handle {fd_data.handle()};

    if (handle == fd_null) {
        return;
    }

    if (fd_data.pending()) {
        const auto verbose {fd_data.verbose()};
        const auto addr {get_sockname(handle, verbose)};

        if (get_sa_family(addr) == AF_UNIX) {
            if (const auto pathname {get_sun_path(addr)}) {
                if (const auto error {unlink(*pathname, verbose)}) {
                    if (error.number() != ENOENT) {
                        std::cerr << *pathname
                                  << ": "
                                  << error.string()
                                  << std::endl;
                    }
                }
            }
        }
    }

#endif
    static_cast<void>(fd_data.close());
}
