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

#include "network/bind-socket-path.h"           // bind()
#include "network/bind-socket-addr.h"           // bind()
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/pathname.h"                   // Pathname
#include "network/socket.h"                     // Socket
#include "network/to-bytestring-path.h"         // to_bytestring()

#ifndef WIN32

auto Network::bind(const Socket& sock,
                   const Pathname& path,
                   bool verbose) -> Network::OsErrorResult
{
    return bind(sock, to_bytestring(path), verbose);
}

#endif
