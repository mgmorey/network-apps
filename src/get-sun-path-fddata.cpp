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

#include "network/get-sun-path-fddata.h"        // FdData,
                                                // OptionalPathname,
                                                // get_sun_path()
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sun-path-bytestring.h"    // get_sun_path()

#ifdef WIN32
#include <winsock2.h>       // AF_UNIX
#else
#include <sys/socket.h>     // AF_UNIX
#endif

auto Network::get_sun_path(const FdData& fd_data,
                           const OptionalPathname& path) ->
    OptionalPathname
{
#ifndef WIN32
    return get_sun_path(fd_data.sockname(), path);
#else
    static_cast<void>(fd_data);
    return path;
#endif
}
